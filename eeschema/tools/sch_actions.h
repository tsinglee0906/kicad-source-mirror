/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2019 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef SCH_ACTIONS_H
#define SCH_ACTIONS_H

#include <tool/tool_action.h>
#include <tool/actions.h>
#include <core/optional.h>

class TOOL_EVENT;
class TOOL_MANAGER;

extern char g_lastBusEntryShape;

/**
 * Class SCH_ACTIONS
 *
 * Gathers all the actions that are shared by tools. The instance of SCH_ACTIONS is created
 * inside of ACTION_MANAGER object that registers the actions.
 */
class SCH_ACTIONS : public ACTIONS
{
public:
    // Selection Tool
    /// Activation of the selection tool
    static TOOL_ACTION selectionActivate;

    /// Select the junction, wire or bus segment under the cursor.
    static TOOL_ACTION selectNode;

    /// If current selection is a wire or bus, expand to entire connection.
    /// Otherwise, select connection under cursor.
    static TOOL_ACTION selectConnection;

    /// Clears the current selection
    static TOOL_ACTION clearSelection;

    /// Selects an item (specified as the event parameter).
    static TOOL_ACTION addItemToSel;
    static TOOL_ACTION removeItemFromSel;

    /// Selects a list of items (specified as the event parameter)
    static TOOL_ACTION addItemsToSel;
    static TOOL_ACTION removeItemsFromSel;

    /// Runs a selection menu to select from a list of items
    static TOOL_ACTION selectionMenu;

    // Locking
    static TOOL_ACTION toggleLock;
    static TOOL_ACTION lock;
    static TOOL_ACTION unlock;

    // Schematic Tools
    static TOOL_ACTION selectionTool;
    static TOOL_ACTION pickerTool;
    static TOOL_ACTION placeSymbol;
    static TOOL_ACTION placePower;
    static TOOL_ACTION startWire;
    static TOOL_ACTION drawWire;
    static TOOL_ACTION startBus;
    static TOOL_ACTION drawBus;
    static TOOL_ACTION unfoldBus;
    static TOOL_ACTION placeNoConnect;
    static TOOL_ACTION placeJunction;
    static TOOL_ACTION placeBusWireEntry;
    static TOOL_ACTION placeBusBusEntry;
    static TOOL_ACTION placeLabel;
    static TOOL_ACTION placeGlobalLabel;
    static TOOL_ACTION placeHierarchicalLabel;
    static TOOL_ACTION drawSheet;
    static TOOL_ACTION resizeSheet;
    static TOOL_ACTION placeSheetPin;
    static TOOL_ACTION importSheetPin;
    static TOOL_ACTION placeSchematicText;
    static TOOL_ACTION startLines;
    static TOOL_ACTION drawLines;
    static TOOL_ACTION placeImage;
    static TOOL_ACTION finishLineWireOrBus;
    static TOOL_ACTION finishWire;
    static TOOL_ACTION finishBus;
    static TOOL_ACTION finishLine;
    static TOOL_ACTION finishSheet;

    // Symbol Tools
    static TOOL_ACTION placeSymbolPin;
    static TOOL_ACTION placeSymbolText;
    static TOOL_ACTION drawSymbolRectangle;
    static TOOL_ACTION drawSymbolCircle;
    static TOOL_ACTION drawSymbolArc;
    static TOOL_ACTION drawSymbolLines;
    static TOOL_ACTION placeSymbolAnchor;

    // Editing
    static TOOL_ACTION move;
    static TOOL_ACTION drag;
    static TOOL_ACTION duplicate;
    static TOOL_ACTION repeatDrawItem;
    static TOOL_ACTION rotateCW;
    static TOOL_ACTION rotateCCW;
    static TOOL_ACTION mirrorX;
    static TOOL_ACTION mirrorY;
    static TOOL_ACTION properties;
    static TOOL_ACTION editReference;
    static TOOL_ACTION editValue;
    static TOOL_ACTION editFootprint;
    static TOOL_ACTION autoplaceFields;
    static TOOL_ACTION convertDeMorgan;
    static TOOL_ACTION editSymbolUnit;
    static TOOL_ACTION doDelete;
    static TOOL_ACTION addJunction;
    static TOOL_ACTION addLabel;
    static TOOL_ACTION addGlobalLabel;
    static TOOL_ACTION addHierLabel;
    static TOOL_ACTION toShapeSlash;
    static TOOL_ACTION toShapeBackslash;
    static TOOL_ACTION toLabel;
    static TOOL_ACTION toHLabel;
    static TOOL_ACTION toGLabel;
    static TOOL_ACTION toText;
    static TOOL_ACTION breakWire;
    static TOOL_ACTION breakBus;

    /// Inspection
    static TOOL_ACTION showDatasheet;
    static TOOL_ACTION showMarkerInfo;
    static TOOL_ACTION editWithSymbolEditor;

    /// Clipboard
    static TOOL_ACTION cut;
    static TOOL_ACTION copy;
    static TOOL_ACTION paste;

    // Miscellaneous
    static TOOL_ACTION enterSheet;
    static TOOL_ACTION leaveSheet;
    static TOOL_ACTION cleanupSheetPins;
    static TOOL_ACTION switchCursor;
    static TOOL_ACTION switchUnits;
    static TOOL_ACTION updateUnits;
    static TOOL_ACTION deleteItemCursor;
    static TOOL_ACTION refreshPreview;
    static TOOL_ACTION explicitCrossProbe;

    // SPICE
    static TOOL_ACTION simProbe;
    static TOOL_ACTION simTune;

    // Net highlighting
    static TOOL_ACTION highlightNet;
    static TOOL_ACTION clearHighlight;
    static TOOL_ACTION highlightNetSelection;
    static TOOL_ACTION highlightNetCursor;

    ///> @copydoc COMMON_ACTIONS::TranslateLegacyId()
    virtual OPT<TOOL_EVENT> TranslateLegacyId( int aId ) override;
};

#endif
