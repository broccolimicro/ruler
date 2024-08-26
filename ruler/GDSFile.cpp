#include "GDSFile.h"

using namespace ruler;

void emitGDS(gdstk::Cell &cell, const Rect &rect, const Layout &layout, int layer) {
	cell.polygon_array.append(new gdstk::Polygon(
		gdstk::rectangle(
			gdstk::Vec2{(double)rect.ll[0], (double)rect.ll[1]},
			gdstk::Vec2{(double)rect.ur[0], (double)rect.ur[1]},
			gdstk::make_tag(layout.tech->paint[layer].major, layout.tech->paint[layer].minor))));

	if (rect.hasLabel()) {
		cell.label_array.append(new gdstk::Label{
			.tag = gdstk::make_tag(layout.tech->paint[layer].major, layout.tech->paint[layer].minor),
			.text = strdup(layout.nets[rect.net].c_str()),
			.origin = gdstk::Vec2{(double)((rect.ll[0] + rect.ur[0])/2), (double)((rect.ll[1]+rect.ur[1])/2)},
			.magnification = 1,
		});
	}
}

void emitGDS(gdstk::Cell &cell, const Layer &layer, const Layout &layout) {
	for (auto r = layer.geo.begin(); r != layer.geo.end(); r++) {
		emitGDS(cell, *r, layout, layer.draw);
	}
}

void emitGDS(gdstk::Library &lib, const Layout &layout) {
	gdstk::Cell *cell = new gdstk::Cell();
	cell->init(layout.name.c_str());
	for (auto layer = layout.layers.begin(); layer != layout.layers.end(); layer++) {
		emitGDS(*cell, *layer, layout);
	}
	lib.cell_array.append(cell);
}


void loadGDS(Layout &layout, const Tech &tech, string path, string cellName) {
	gdstk::Library lib = gdstk::read_gds(path.c_str(), tech.dbunit*1e-6, tech.dbunit*1e-6, nullptr, nullptr);
	gdstk::Cell *gdsCell = lib.get_cell(cellName.c_str());
	if (gdsCell == nullptr) {
		return;
	}

	int polyCount = 0;

	gdstk::Array<gdstk::Polygon*> polys;
	gdsCell->get_polygons(true, true, -1, false, gdstk::Tag{}, polys);

	for (int i = 0; i < (int)polys.count; i++) {
		gdstk::Polygon* poly = polys[i];
		if (poly->point_array.count != 4) {
			polyCount++;
			continue;
		}

		vec2i ll(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
		vec2i ur(std::numeric_limits<double>::min(), std::numeric_limits<double>::min());
	
		int major = gdstk::get_layer(poly->tag);
		int minor = gdstk::get_type(poly->tag);

		int draw = tech.findPaint(major, minor);
		for (int j = 0; j < (int)poly->point_array.count; j++) {
			gdstk::Vec2 point = poly->point_array[j];
			if (point.x < ll[0]) {
				ll[0] = point.x;
			}
			if (point.x > ur[0]) {
				ur[0] = point.x;
			}
			if (point.y < ll[1]) {
				ll[1] = point.y;
			}
			if (point.y > ur[1]) {
				ur[1] = point.y;
			}			
		}

		layout.push(draw, Rect(-1, ll, ur));
	}
	if (polyCount > 0) {
		printf("found %d polygons, only rectangles are supported\n", polyCount);
	}

	lib.free_all();
}

