/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * Additional copyright for this file:
 * Copyright (C) 1994-1998 Revolution Software Ltd.
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
 */

#ifndef SWORD2_DETECTION_H
#define SWORD2_DETECTION_H

#include "engines/advancedDetector.h"

namespace Sword2 {

enum {
	GF_DEMO	       = 1 << 0,
	GF_SPANISHDEMO = 1 << 1
};

struct Sword2GameDescription {
	ADGameDescription desc;

	uint32 features;
};

#define GAMEOPTION_OBJECT_LABELS GUIO_GAMEOPTIONS1

} // End of namespace Sword2

#endif // SWORD2_DETECTION_H
