/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Font management and font drawing header file

#ifndef SAGA_FONT_H
#define SAGA_FONT_H

#include "common/list.h"
#include "saga/gfx.h"

namespace Graphics {
	class FontSJIS;
}

namespace Saga {

#define FONT_SHOWUNDEFINED 1	// Define to draw undefined characters * as ?'s

// The first defined character (!) is the only one that may
// have a valid offset of '0'
#define FONT_FIRSTCHAR 33

#define FONT_CH_TAB 9
#define FONT_CH_SPACE 32
#define FONT_CH_QMARK 63

// Minimum font header size without font data
// (6 + 512 + 256 + 256 + 256 )
#define FONT_DESCSIZE 1286

#define FONT_CHARCOUNT 256
#define FONT_CHARMASK 0xFFU

#define SAGA_FONT_HEADER_LEN 6

#define TEXT_CENTERLIMIT 50
#define TEXT_MARGIN 10
#define TEXT_LINESPACING 2

enum FontEffectFlags {
	kFontNormal   = 0,
	kFontOutline  = 1 << 0,
	kFontShadow   = 1 << 1,
	kFontBold     = 1 << 2,
	kFontCentered = 1 << 3,
	kFontDontmap  = 1 << 4
};

enum KnownFont {
	kKnownFontSmall,
	kKnownFontMedium,
	kKnownFontBig,

	kKnownFontPause,
	kKnownFontScript,
	kKnownFontVerb
};

struct TextListEntry {
	bool display;
	bool useRect;
	Common::Point point;
	Common::Rect rect;
	KnownColor knownColor;
	KnownColor effectKnownColor;
	FontEffectFlags flags;
	KnownFont font;
	const char *text;

	TextListEntry() {
		display = false;
		useRect = false;
		// point initialized by Common::Point constructor
		// rect initialized by Common::Rect constructor
		knownColor = kKnownColorTransparent;
		effectKnownColor = kKnownColorTransparent;
		flags = kFontNormal;
		font = kKnownFontSmall;
		text = nullptr;
	}

	bool operator==(const TextListEntry &e) const {
		return 0 == memcmp(this, &e, sizeof(*this));
	}
};

class TextList: public Common::List<TextListEntry> {
public:

	TextListEntry *addEntry(const TextListEntry &entry) {
		Common::List<TextListEntry>::push_back(entry);
		return &*--Common::List<TextListEntry>::end();
	}
};

struct FontHeader {
	int charHeight;
	int charWidth;
	int rowLength;
};

struct FontCharEntry {
	int index;
	int byteWidth;
	int width;
	int flag;
	int tracking;
};

struct FontStyle {
	FontHeader header;
	FontCharEntry fontCharEntry[256];
#ifndef __DS__
	ByteArray font;
#else
	byte* font;
#endif
};

struct FontData {
	FontStyle normal;
	FontStyle outline;
};

class Font {
public:
	Font(SagaEngine *vm) : _vm(vm) {}
	virtual ~Font() {}

	int getStringWidth(KnownFont font, const char *text, size_t count, FontEffectFlags flags) {
		return getStringWidth(knownFont2FontIdx(font), text, count, flags);
	}

	int getHeight(KnownFont font) {
		return getHeight(knownFont2FontIdx(font));
	}

	int getHeight(KnownFont font, const char *text, int width, FontEffectFlags flags) {
		return getHeight(knownFont2FontIdx(font), text, width, flags);
	}

	void textDraw(KnownFont font, const char *string, const Common::Point &point, int color, int effectColor, FontEffectFlags flags) {
		textDraw(knownFont2FontIdx(font), string, point, color, effectColor, flags);
	}

	void textDrawRect(KnownFont font, const char *text, const Common::Rect &rect, int color, int effectColor, FontEffectFlags flags) {
		textDrawRect(knownFont2FontIdx(font), text, rect, color, effectColor, flags);
	}

	virtual void setFontMapping(int) {}

protected:
	enum FontId {
		kSmallFont,
		kMediumFont,
		kBigFont,
		kIHNMUnknown,
		kIHNMFont8,
		kIHNMUnknown2,
		kIHNMMainFont
	};

	SagaEngine *_vm;

private:
	FontId knownFont2FontIdx(KnownFont font);
	void textDraw(FontId fontId, const char *string, const Common::Point &point, int color, int effectColor, FontEffectFlags flags);

	virtual void textDrawRect(FontId fontId, const char *text, const Common::Rect &rect, int color, int effectColor, FontEffectFlags flags) = 0;
	virtual int translateChar(int charId) = 0;
	virtual int getStringLength(const char *text) = 0;
	virtual int getStringWidth(FontId fontId, const char *text, size_t count, FontEffectFlags flags) = 0;
	virtual int getHeight(FontId fontId, const char *text, int width, FontEffectFlags flags) = 0;
	virtual int getHeight(FontId fontId) = 0;
	virtual bool valid(FontId) = 0;
	virtual void draw(FontId fontId, const char *text, size_t count, const Common::Point &point, int color, int effectColor, FontEffectFlags flags) = 0;
};

class DefaultFont : public Font {
 public:
	DefaultFont(SagaEngine *vm);
	~DefaultFont() override;

	void setFontMapping(int mapping) override {
		_fontMapping = mapping;
	}

 private:
	 void textDrawRect(FontId fontId, const char *text, const Common::Rect &rect, int color, int effectColor, FontEffectFlags flags) override;
	 int translateChar(int charId) override;
	 int getStringLength(const char *text) override {
		 return strlen(text);
	 }

	 int getStringWidth(FontId fontId, const char *text, size_t count, FontEffectFlags flags) override;
	 int getHeight(FontId fontId, const char *text, int width, FontEffectFlags flags) override;
	 int getHeight(FontId fontId) override {
		 return getFont(fontId)->normal.header.charHeight;
	 }

	 int getHeight(FontId fontId, const char *text);

	 void validate(FontId fontId) {
		 if (!valid(fontId)) {
			 error("Font::validate: Invalid font id");
		 }
	 }

	 bool valid(FontId fontId) override {
		 return (uint(fontId) < _fonts.size());
	 }

	 FontData *getFont(FontId fontId) {
		 validate(fontId);
		 return &_fonts[fontId];
	 }

	 void draw(FontId fontId, const char *text, size_t count, const Common::Point &point, int color, int effectColor, FontEffectFlags flags) override;
	 void outFont(const FontStyle &drawFont, const char *text, size_t count, const Common::Point &point, int color, FontEffectFlags flags);
	 void loadFont(FontData *font, uint32 fontResourceId);
	 void loadChineseFontIHNM(FontData *font, uint32 fontResourceId);
	 void loadChineseFontITE(const Common::String& fileName);
	 void loadKoreanFontIHNM(const Common::String& fileName);
	 void saveBig5Index(byte head, byte tail, uint curIdx);
	 void createOutline(FontData *font);
	 void blitGlyph(const Common::Point &textPoint, const byte* bitmap, int charWidth, int charHeight, int rowLength, byte color);

	 int getByteLen(int numBits) const {
		 int byteLength = numBits / 8;

		 if (numBits % 8) {
			 byteLength++;
		 }

		 return byteLength;
	 }

	static const int _charMap[128];

	Common::Array<FontData> _fonts;
	int _fontMapping;

	byte *_chineseFont;
	Common::Array<int> _chineseFontIndex;
	int _cjkFontWidth;
	int _cjkFontHeight;

	byte *_koreanFont;

	static const int kIHNMKoreanGlyphBytes = 32;
	static const int kIHNMKoreanInitials = 20;
	static const int kIHNMKoreanInitialVariants = 8;
	static const int kIHNMKoreanMidOffset = kIHNMKoreanInitialVariants * kIHNMKoreanInitials;
	static const int kIHNMKoreanMids = 22;
	static const int kIHNMKoreanMidVariants = 4;
	static const int kIHNMKoreanFinalsOffset = kIHNMKoreanMidOffset + kIHNMKoreanMids * kIHNMKoreanMidVariants;
	static const int kIHNMKoreanFinals = 28;
	static const int kIHNMKoreanFinalVariants = 4;
	static const int kIHNMKoreanNonJamoOffset = kIHNMKoreanFinalsOffset + kIHNMKoreanFinals * kIHNMKoreanFinalVariants;
	static const int kIHNMKoreanNonJamo = 126;
};

class SJISFont : public Font {
public:
	SJISFont(SagaEngine *vm);
	~SJISFont() override;

private:
	void textDrawRect(FontId fontId, const char *text, const Common::Rect &rect, int color, int effectColor, FontEffectFlags flags) override;
	int translateChar(int charId) override { return charId; }
	int getStringLength(const char *text) override;
	int getStringWidth(FontId fontId, const char *text, size_t count, FontEffectFlags flags) override;
	int getHeight(FontId fontId, const char *text, int width, FontEffectFlags flags) override;
	int getHeight(FontId fontId) override;
	bool valid(FontId fontId) override { return fontId != kBigFont; }

	void draw(FontId fontId, const char *text, size_t count, const Common::Point &point, int color, int effectColor, FontEffectFlags flags) override;

	uint16 fetchChar(const char *&s) const;
	bool preventLineBreakForCharacter(uint16 ch) const;

	Graphics::FontSJIS *_font;
};

} // End of namespace Saga

#endif
