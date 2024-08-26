#pragma once

#include <gdstk/gdstk.hpp>
#include "Layout.h"

using namespace ruler;

void emitGDS(gdstk::Cell &cell, const Rect &rect, const Layout &layout, int layer);
void emitGDS(gdstk::Cell &cell, const Layer &layer, const Layout &layout);
void emitGDS(gdstk::Library &lib, const Layout &layout);

void loadGDS(Layout &layout, const Tech &tech, string path, string cellName);
