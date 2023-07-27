#ifndef QUADFALL_CELL_HPP
#define QUADFALL_CELL_HPP

#include <raylib-cpp.hpp>

namespace qf
{
    enum CellColor {
        // transparent
        QC_BLANK,
        // #FFF
        QC_WHITE,
        // #BBB (silver)
        QC_GRAY_L,
        // #888
        QC_GRAY,
        // #444
        QC_GRAY_D,
        // #000
        QC_BLACK,
        // #F00
        QC_RED,
        // #F80
        QC_ORANGE,
        // #FF0
        QC_YELLOW,
        // #080
        QC_GREEN,
        // #0F0
        QC_LIME,
        // #0FF
        QC_CYAN,
        // #00F
        QC_BLUE,
        // #808
        QC_PURPLE,
        // #F0F
        QC_MAGENTA,
        // #800
        QC_MAROON,
    };

    raylib::Color QcToRaylibColor(CellColor c) {
        using RC = raylib::Color;
        switch (c)
        {
        case QC_WHITE: return RC::White();
        case QC_GRAY_L: return RC::LightGray();
        case QC_GRAY: return RC::Gray();
        case QC_GRAY_D: return RC::DarkGray();
        // #000
        case QC_BLACK: return RC::Black();
        // #F00
        case QC_RED: return RC::Red();
        // #F80
        case QC_ORANGE: return RC::Orange();
        // #FF0
        case QC_YELLOW: return RC::Yellow();
        // #080
        case QC_GREEN: return RC::Green();
        // #0F0
        case QC_LIME: return RC::Lime();
        // #0FF
        case QC_CYAN: return RC::SkyBlue();
        // #00F
        case QC_BLUE: return RC::Blue();
        // #808
        case QC_PURPLE: return RC::Purple();
        // #F0F
        case QC_MAGENTA: return RC::Magenta();
        // #800
        case QC_MAROON: return RC::Maroon();
        case QC_BLANK:
        default: return RC::Blank();
        }
    }

    struct Cell {
        int weight;
        CellColor color;

        bool empty() { return weight == 0; }
        CellColor getColor() { return empty() ? CellColor::QC_BLANK : 
                                      color == 0 ? CellColor::QC_WHITE : color; }
        void draw(raylib::Rectangle rec) {
            rec.DrawLines(raylib::Color::White());
            rec.Draw(QcToRaylibColor(getColor()));
        }
        void draw(float x, float y, float size) { draw(raylib::Rectangle{x, y, size, size}); }

        static bool isEmpty(Cell &c) { return c.empty(); }
        static bool isFilled(Cell &c) { return !c.empty(); }

        Cell() : Cell{0, QC_BLANK} {};
        Cell(int w, CellColor c = CellColor::QC_WHITE) : weight{w}, color{c} {}
    };
    
} // namespace qf

#endif // !QUADFALL_CELL_HPP

