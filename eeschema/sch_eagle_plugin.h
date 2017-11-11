/*
* This program source code file is part of KiCad, a free EDA CAD application.
*
* Copyright (C) 2017 CERN
* @author Alejandro García Montoro <alejandro.garciamontoro@gmail.com>
* @author Maciej Suminski <maciej.suminski@cern.ch>
* @author Russell Oliver <roliver8143@gmail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 3
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SCH_EAGLE_PLUGIN_H_
#define _SCH_EAGLE_PLUGIN_H_

#include <wx/xml/xml.h>

#include <sch_line.h>
#include <sch_io_mgr.h>
#include <eagle_parser.h>
#include <lib_draw_item.h>
#include <dlist.h>

#include <boost/ptr_container/ptr_map.hpp>

class EDA_TEXT;
class KIWAY;
class LINE_READER;
class SCH_SCREEN;
class SCH_SHEET;
class SCH_BITMAP;
class SCH_JUNCTION;
class SCH_NO_CONNECT;
class SCH_LINE;
class SCH_BUS_ENTRY_BASE;
class SCH_TEXT;
class SCH_GLOBALLABEL;
class SCH_COMPONENT;
class SCH_FIELD;
class PROPERTIES;
class SCH_EAGLE_PLUGIN_CACHE;
class LIB_PART;
class PART_LIB;
class LIB_ALIAS;
class LIB_CIRCLE;
class LIB_FIELD;
class LIB_RECTANGLE;
class LIB_POLYLINE;
class LIB_PIN;
class LIB_TEXT;


typedef struct EAGLE_LIBRARY
{
    std::string name;
    boost::ptr_map<std::string, LIB_PART> KiCadSymbols;
    std::unordered_map<std::string, wxXmlNode*> SymbolNodes;
    std::unordered_map<std::string, int> GateUnit;
    std::unordered_map<std::string, std::string> package;
} EAGLE_LIBRARY;

typedef boost::ptr_map<std::string, EPART> EPART_LIST;


/**
 * Class SCH_EAGLE_PLUGIN
 * is a #SCH_PLUGIN derivation for loading 6.x+ Eagle schematic files.
 *
 * As with all SCH_PLUGINs there is no UI dependencies i.e. windowing calls allowed.
 */
class SCH_EAGLE_PLUGIN : public SCH_PLUGIN
{
public:
    SCH_EAGLE_PLUGIN();
    ~SCH_EAGLE_PLUGIN();

    const wxString GetName() const override;

    const wxString GetFileExtension() const override;

    int GetModifyHash() const override;

    SCH_SHEET* Load( const wxString& aFileName, KIWAY* aKiway, SCH_SHEET* aAppendToMe = NULL,
                     const PROPERTIES* aProperties = NULL ) override;

    bool CheckHeader( const wxString& aFileName ) override;


    // unimplemented functions. Will trigger a not_implemented IO error.
    //void SaveLibrary( const wxString& aFileName, const PROPERTIES* aProperties = NULL ) override;

    //void Save( const wxString& aFileName, SCH_SCREEN* aSchematic, KIWAY* aKiway,
    //           const PROPERTIES* aProperties = NULL ) override;

    //size_t GetSymbolLibCount( const wxString& aLibraryPath,
    //                          const PROPERTIES* aProperties = NULL ) override;

    //void EnumerateSymbolLib( wxArrayString& aAliasNameList, const wxString& aLibraryPath,
    //                         const PROPERTIES* aProperties = NULL ) override;

    //LIB_ALIAS* LoadSymbol( const wxString& aLibraryPath, const wxString& aAliasName,
    //                       const PROPERTIES* aProperties = NULL ) override;

    //void SaveSymbol( const wxString& aLibraryPath, const LIB_PART* aSymbol,
    //                 const PROPERTIES* aProperties = NULL ) override;

    //void DeleteAlias( const wxString& aLibraryPath, const wxString& aAliasName,
    //                  const PROPERTIES* aProperties = NULL ) override;

    //void DeleteSymbol( const wxString& aLibraryPath, const wxString& aAliasName,
    //                   const PROPERTIES* aProperties = NULL ) override;

    //void CreateSymbolLib( const wxString& aLibraryPath,
    //                      const PROPERTIES* aProperties = NULL ) override;

    // bool DeleteSymbolLib( const wxString& aLibraryPath,
    //                      const PROPERTIES* aProperties = NULL ) override;

    //bool IsSymbolLibWritable( const wxString& aLibraryPath ) override;

    //void SymbolLibOptions( PROPERTIES* aListToAppendTo ) const override;

private:
    void loadDrawing( wxXmlNode* aDrawingNode );
    void loadLayerDefs( wxXmlNode* aLayers );
    void loadSchematic( wxXmlNode* aSchematicNode );
    void loadSheet( wxXmlNode* aSheetNode, int sheetcount );
    void loadInstance( wxXmlNode* aInstanceNode );
    EAGLE_LIBRARY* loadLibrary( wxXmlNode* aLibraryNode, EAGLE_LIBRARY* aEagleLib );
    void countNets( wxXmlNode* aSchematicNode );

    /// Moves any labels on the wire to the new end point of the wire.
    void moveLabels( SCH_ITEM* aWire, const wxPoint& aNewEndPoint );

    /// This function finds best way to place a bus entry symbol for when an Eagle wire segment
    /// ends on an Eagle bus segment.
    void addBusEntries();

    ///> Translates Eagle special characters to their counterparts in KiCad.
    static wxString escapeName( const wxString& aNetName );

    /// Return the matching layer or return LAYER_NOTES
    SCH_LAYER_ID kiCadLayer( int aEagleLayer );

    wxPoint findNearestLinePoint( const wxPoint& aPoint, const DLIST<SCH_LINE>& aLines );

    void                loadSegments( wxXmlNode* aSegmentsNode, const wxString& aNetName,
                                      const wxString& aNetClass );
    SCH_LINE*           loadWire( wxXmlNode* aWireNode );
    SCH_TEXT*           loadLabel( wxXmlNode* aLabelNode, const wxString& aNetName, const DLIST< SCH_LINE >& segmentWires);
    SCH_JUNCTION*       loadJunction( wxXmlNode* aJunction );
    SCH_TEXT*           loadPlainText( wxXmlNode* aSchText );

    bool            loadSymbol( wxXmlNode* aSymbolNode, std::unique_ptr<LIB_PART>& aPart, EDEVICE* aDevice, int aGateNumber, string aGateName );
    LIB_CIRCLE*     loadSymbolCircle( std::unique_ptr<LIB_PART>& aPart, wxXmlNode* aCircleNode, int aGateNumber );
    LIB_RECTANGLE*  loadSymbolRectangle( std::unique_ptr<LIB_PART>& aPart, wxXmlNode* aRectNode, int aGateNumber );
    LIB_POLYLINE*   loadSymbolPolyLine( std::unique_ptr<LIB_PART>& aPart, wxXmlNode* aPolygonNode, int aGateNumber );
    LIB_ITEM*       loadSymbolWire( std::unique_ptr<LIB_PART>& aPart, wxXmlNode* aWireNode, int aGateNumber );
    LIB_PIN*        loadPin( std::unique_ptr<LIB_PART>& aPart, wxXmlNode*, EPIN* epin, int aGateNumber );
    LIB_TEXT*       loadSymbolText( std::unique_ptr<LIB_PART>& aPart, wxXmlNode* aLibText, int aGateNumber );

    void            loadTextAttributes( EDA_TEXT* aText, const ETEXT& aAttribs ) const;
    void            loadFieldAttributes( LIB_FIELD* aField, const LIB_TEXT* aText ) const;

    wxString        getLibName() const;
    wxFileName      getLibFileName() const;

    KIWAY* m_kiway;      ///< For creating sub sheets.
    SCH_SHEET* m_rootSheet; ///< The root sheet of the schematic being loaded..
    SCH_SHEET* m_currentSheet; ///< The current sheet of the schematic being loaded..
    wxString m_version; ///< Eagle file version.
    wxFileName m_filename;

    EPART_MAP m_partlist;
    std::map<std::string, EAGLE_LIBRARY> m_eagleLibs;

    SCH_PLUGIN::SCH_PLUGIN_RELEASER m_pi;         ///< Plugin to create the KiCad symbol library.
    std::unique_ptr< PROPERTIES > m_properties;   ///< Library plugin properties.

    std::map<std::string, int> m_netCounts;
    std::map<int, SCH_LAYER_ID> m_layerMap;
};

#endif  // _SCH_EAGLE_PLUGIN_H_