/***********************************************************************/
/* component_class.cpp : handle the  class SCH_COMPONENT  */
/***********************************************************************/

#include "fctsys.h"
#include "gr_basic.h"

#include "common.h"
#include "program.h"
#include "libcmp.h"
#include "general.h"
#include "macros.h"

#include "protos.h"


/***************************************************************************/
SCH_CMP_FIELD::SCH_CMP_FIELD( const wxPoint& pos, const wxString& text ) :
    SCH_ITEM( NULL, DRAW_PART_TEXT_STRUCT_TYPE ),
    EDA_TextStruct( text )
/***************************************************************************/
{
    m_Pos          = pos;
    m_FieldId      = 0;
    m_AddExtraText = false;
}


/************************************/
SCH_CMP_FIELD::~SCH_CMP_FIELD()
/************************************/
{
}


/**************************************************************************/
void SCH_CMP_FIELD::SwapData( SCH_CMP_FIELD* copyitem )
/**************************************************************************/

/* Used if undo / redo command:
 *  swap data between this and copyitem
 */
{
    EXCHG( m_Text, copyitem->m_Text );
    EXCHG( m_Layer, copyitem->m_Layer );
    EXCHG( m_Pos, copyitem->m_Pos );
    EXCHG( m_Size, copyitem->m_Size );
    EXCHG( m_Width, copyitem->m_Width );
    EXCHG( m_Orient, copyitem->m_Orient );
    EXCHG( m_Miroir, copyitem->m_Miroir );
    EXCHG( m_Attributs, copyitem->m_Attributs );
    EXCHG( m_CharType, copyitem->m_CharType );
    EXCHG( m_HJustify, copyitem->m_HJustify );
    EXCHG( m_VJustify, copyitem->m_VJustify );
    EXCHG( m_ZoomLevelDrawable, copyitem->m_ZoomLevelDrawable );
    EXCHG( m_TextDrawings, copyitem->m_TextDrawings );
    EXCHG( m_TextDrawingsSize, copyitem->m_TextDrawingsSize );
}


/***********************************************************/
void SCH_CMP_FIELD::PartTextCopy( SCH_CMP_FIELD* target )
/***********************************************************/
{
    target->m_Text = m_Text;
    if( m_FieldId >= FIELD1 )
        target->m_Name = m_Name;
    target->m_Layer     = m_Layer;
    target->m_Pos       = m_Pos;
    target->m_Size      = m_Size;
    target->m_Attributs = m_Attributs;
    target->m_FieldId   = m_FieldId;
    target->m_Orient    = m_Orient;
    target->m_HJustify  = m_HJustify;
    target->m_VJustify  = m_VJustify;
    target->m_Flags     = m_Flags;
}


/*********************************/
bool SCH_CMP_FIELD::IsVoid()
/*********************************/

/* return True if the field is void, i.e.:
 *  contains  "~" or ""
 */
{
    if( m_Text.IsEmpty() || m_Text == wxT( "~" ) )
        return TRUE;
    return FALSE;
}


/********************************************/
EDA_Rect SCH_CMP_FIELD::GetBoundaryBox() const
/********************************************/

/** Function GetBoundaryBox()
 * @return an EDA_Rect contains the real (user coordinates) boundary box for a text field,
 *  according to the component position, rotation, mirror ...
 *
 */
{
    EDA_Rect       BoundaryBox;
    int            hjustify, vjustify;
    int            textlen;
    int            orient;
    int            dx, dy, x1, y1, x2, y2;

    SCH_COMPONENT* DrawLibItem = (SCH_COMPONENT*) m_Parent;

    orient = m_Orient;
    wxPoint        pos = DrawLibItem->m_Pos;
    x1 = m_Pos.x - pos.x;
    y1 = m_Pos.y - pos.y;

    textlen = GetLength();
    if( m_FieldId == REFERENCE )   // Real Text can be U1 or U1A
    {
        EDA_LibComponentStruct* Entry =
            FindLibPart( DrawLibItem->m_ChipName.GetData(), wxEmptyString, FIND_ROOT );
        if( Entry && (Entry->m_UnitCount > 1) )
            textlen++; // because U1 is show as U1A or U1B ...
    }
    dx = m_Size.x * textlen;

    // Real X Size is 10/9 char size because space between 2 chars is 1/10 X Size
    dx = (dx * 10) / 9;

    dy = m_Size.y;
    hjustify = m_HJustify;
    vjustify = m_VJustify;

    x2 = pos.x + (DrawLibItem->m_Transform[0][0] * x1)
         + (DrawLibItem->m_Transform[0][1] * y1);
    y2 = pos.y + (DrawLibItem->m_Transform[1][0] * x1)
         + (DrawLibItem->m_Transform[1][1] * y1);

    /* If the component orientation is +/- 90 deg, the text orienation must be changed */
    if( DrawLibItem->m_Transform[0][1] )
    {
        if( orient == TEXT_ORIENT_HORIZ )
            orient = TEXT_ORIENT_VERT;
        else
            orient = TEXT_ORIENT_HORIZ;
        /* is it mirrored (for text justify)*/
        EXCHG( hjustify, vjustify );
        if( DrawLibItem->m_Transform[1][0] < 0 )
            vjustify = -vjustify;
        if( DrawLibItem->m_Transform[0][1] > 0 )
            hjustify = -hjustify;
    }
    else    /* component horizontal: is it mirrored (for text justify)*/
    {
        if( DrawLibItem->m_Transform[0][0] < 0 )
            hjustify = -hjustify;
        if( DrawLibItem->m_Transform[1][1] > 0 )
            vjustify = -vjustify;
    }

    if( orient == TEXT_ORIENT_VERT )
        EXCHG( dx, dy );

    switch( hjustify )
    {
    case GR_TEXT_HJUSTIFY_CENTER:
        x1 = x2 - (dx / 2);
        break;

    case GR_TEXT_HJUSTIFY_RIGHT:
        x1 = x2 - dx;
        break;

    default:
        x1 = x2;
        break;
    }

    switch( vjustify )
    {
    case GR_TEXT_VJUSTIFY_CENTER:
        y1 = y2 - (dy / 2);
        break;

    case GR_TEXT_VJUSTIFY_BOTTOM:
        y1 = y2 - dy;
        break;

    default:
        y1 = y2;
        break;
    }

    BoundaryBox.SetX( x1 );
    BoundaryBox.SetY( y1 );
    BoundaryBox.SetWidth( dx );
    BoundaryBox.SetHeight( dy );

    return BoundaryBox;
}


/**
 * Function Save
 * writes the data structures for this object out to a FILE in "*.brd" format.
 * @param aFile The FILE to write to.
 * @return bool - true if success writing else false.
 */
bool SCH_CMP_FIELD::Save( FILE* aFile ) const
{
    char hjustify = 'C';

    if( m_HJustify == GR_TEXT_HJUSTIFY_LEFT )
        hjustify = 'L';
    else if( m_HJustify == GR_TEXT_HJUSTIFY_RIGHT )
        hjustify = 'R';
    char vjustify = 'C';
    if( m_VJustify == GR_TEXT_VJUSTIFY_BOTTOM )
        vjustify = 'B';
    else if( m_VJustify == GR_TEXT_VJUSTIFY_TOP )
        vjustify = 'T';
    if( fprintf( aFile, "F %d \"%s\" %c %-3d %-3d %-3d %4.4X %c %c", m_FieldId,
            CONV_TO_UTF8( m_Text ),
            m_Orient == TEXT_ORIENT_HORIZ ? 'H' : 'V',
            m_Pos.x, m_Pos.y,
            m_Size.x,
            m_Attributs,
            hjustify, vjustify ) == EOF )
    {
        return false;
    }


    // Save field name, if necessary
    if( m_FieldId >= FIELD1 && !m_Name.IsEmpty() )
    {
        wxString fieldname = ReturnDefaultFieldName( m_FieldId );
        if( fieldname != m_Name )
        {
            if( fprintf( aFile, " \"%s\"", CONV_TO_UTF8( m_Name ) ) == EOF )
            {
                return false;
            }
        }
    }

    if( fprintf( aFile, "\n" ) == EOF )
    {
        return false;
    }

    return true;
}
