// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#include "q4max_visibilitypropertiesctrl.h"

#include "common.h"
#include "mainframe.h"
#include "hudfilebase.h"
#include "elementsctrlbase.h"
#include "propertiesnotebookbase.h"
#include "artprovider.h"
#include "q4max_element.h"



BEGIN_EVENT_TABLE(Q4MAXVisibilityPropertiesCtrl, Q4MAXPropertyGrid)
  EVT_PG_CHANGED(ID_NOTEBOOK_PROPERTIES, Q4MAXVisibilityPropertiesCtrl::OnItemChanged)
  EVT_BUTTON(ID_NOTEBOOK_PROPERTIES, Q4MAXVisibilityPropertiesCtrl::OnBtn)
  EVT_TOOL_RANGE(ID_BTN_ALIGN_LEFT, ID_BTN_ALIGN_BOTTOM, Q4MAXVisibilityPropertiesCtrl::OnAlign)
  EVT_TOOL_RANGE(ID_BTN_ELEMENT_ENABLE, ID_BTN_ELEMENT_DISABLE, Q4MAXVisibilityPropertiesCtrl::OnElementVisibility)
END_EVENT_TABLE()

#include "resetbuttoneditor.h"

//WX_PG_DECLARE_STRING_PROPERTY(wxTestStringProperty)
//WX_PG_IMPLEMENT_STRING_PROPERTY(wxTestStringProperty,wxPG_NO_ESCAPE)


Q4MAXVisibilityPropertiesCtrl::Q4MAXVisibilityPropertiesCtrl( wxWindow *parent ) :
  Q4MAXPropertyGrid( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, // position
            wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|wxPG_DESCRIPTION|wxPG_TOOLBAR|wxPGMAN_DEFAULT_STYLE )
{
  wxPGRegisterEditorClass( ResetButtonEditor );
  // needed for `time' (duration) to give the user possibility to revert back to inherital value
  // TODO we could also add a [x] button that goes back to inherited!
  SetExtraStyle(wxPG_EX_AUTO_UNSPECIFIED_VALUES); 

  AddPage(_("Visibility"));

  // a value of 0 (=disable) isn't the same as unspecified (=inherit)
  Append( new wxIntProperty( _("Duration [ms]"), wxT("time"), 0) );
  SetPropertyEditor(wxT("time"),wxPG_EDITOR(SpinCtrl));
  //SetPropertyEditor(wxT("time"),wxPG_EDITOR(SCResetButtonEditor));
  SetPropertyHelpString( wxT("time"), _("How long the element will be displayed for if it doesn't update again. Generally used for item pickups, frag messages, chat, etc.\n\nClear to disable.") );

  // Register editor class - needs only to be called once
  /*
  wxPGChoices visible_choices;
  visible_choices.Add(wxT("DUEL"), 1);
  visible_choices.Add(wxT("TDM"), 2);
  visible_choices.Add(wxT("CTF"), 4);
  Append( new wxMultiChoiceProperty(_("Gametypes"), wxT("visible"), visible_choices) );
  SetPropertyEditor(wxT("visible"),wxPG_EDITOR(ResetButtonEditor));
  */
  Append( new wxPropertyCategory( _("Gametypes"), wxT("cat-visible")) );
  Append( new wxBoolProperty( _("Force hidden"), wxT("visible-forcehidden"), false) );
  Append( new wxBoolProperty( _("DUEL"), wxT("visible-duel"), false) );
  Append( new wxBoolProperty( _("TDM"), wxT("visible-tdm"), false) );
  Append( new wxBoolProperty( _("CTF"), wxT("visible-ctf"), false) );

  Append( new wxPropertyCategory( _("Position"), wxT("cat-pos")) );
   
  Append( new wxBoolProperty( _("Use"), wxT("overwrite-pos"), false) );
  SetPropertyHelpString( wxT("overwrite-pos"), _("By default an element is drawn at position (0,0). Check this box to specify your own values.") );

  Append( new wxIntProperty( wxT("X"), wxPG_LABEL, 0) );
  SetPropertyHelpString( wxT("X"), _("This sets where the element is drawn, how many pixels from left") );
  SetPropertyEditor(wxT("X"),wxPG_EDITOR(SpinCtrl));

  Append( new wxIntProperty( wxT("Y"), wxPG_LABEL, 0) );
  SetPropertyHelpString( wxT("Y"), _("This sets where the element is drawn, how many pixels from top") );
  SetPropertyEditor(wxT("Y"),wxPG_EDITOR(SpinCtrl));

  Append( new wxPropertyCategory( _("Dimensions"), wxT("cat-dim")) );

  Append( new wxBoolProperty( _("Use"), wxT("overwrite-dim"), false) );
  Append( new wxIntProperty( _("Width"), wxT("width"), 0) );
  SetPropertyEditor(wxT("width"),wxPG_EDITOR(SpinCtrl));

  Append( new wxIntProperty( _("Height"), wxT("height"), 0) );
  SetPropertyEditor(wxT("height"),wxPG_EDITOR(SpinCtrl));

  SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,(long)1);

  wxToolBar *tb = GetToolBar();
  // we ain't interested in the default buttons :o
  while( tb->GetToolsCount() )
    tb->DeleteToolByPos(0);
  tb->AddRadioTool( ID_BTN_ELEMENT_ENABLE, _("Enable element"), wxArtProvider::GetBitmap(ART_ELEMENT_ENABLED, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ELEMENT_ENABLE, _("Enable element") );
  tb->AddRadioTool( ID_BTN_ELEMENT_DISABLE, _("Disable element"), wxArtProvider::GetBitmap(ART_ELEMENT_DISABLED, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ELEMENT_DISABLE, _("Disable element") );
  tb->AddSeparator();
  tb->AddTool( ID_BTN_ALIGN_LEFT, _("Align element left"), wxArtProvider::GetBitmap(ART_ALIGN_LEFT, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_LEFT, _("Align element left") );
  tb->AddTool( ID_BTN_ALIGN_CENTER, _("Align element center"), wxArtProvider::GetBitmap(ART_ALIGN_CENTER, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_CENTER, _("Align element center") );
  tb->AddTool( ID_BTN_ALIGN_RIGHT, _("Align element right"), wxArtProvider::GetBitmap(ART_ALIGN_RIGHT, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_RIGHT, _("Align element right") );
  tb->AddSeparator();
  tb->AddTool( ID_BTN_ALIGN_TOP, _("Align element top"), wxArtProvider::GetBitmap(ART_ALIGN_TOP, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_TOP, _("Align element top") );
  tb->AddTool( ID_BTN_ALIGN_MIDDLE, _("Align element middle"), wxArtProvider::GetBitmap(ART_ALIGN_MIDDLE, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_MIDDLE, _("Align element middle") );
  tb->AddTool( ID_BTN_ALIGN_BOTTOM, _("Align element bottom"), wxArtProvider::GetBitmap(ART_ALIGN_BOTTOM, wxART_TOOLBAR, wxSize(16,16)) );
  tb->SetToolShortHelp( ID_BTN_ALIGN_BOTTOM, _("Align element bottom") );
  tb->Realize();
}




void Q4MAXVisibilityPropertiesCtrl::ExpandAll( bool expand /*=true*/ )
{
  Expand(wxT("cat-pos"), expand);
  Expand(wxT("cat-dim"), expand);
  Expand(wxT("cat-visible"), expand);
}

void Q4MAXVisibilityPropertiesCtrl::OnBtn( wxCommandEvent& ev )
{
  // get selected property
  wxString prop = GetSelectedProperty()->GetName();
  wxLogDebug(wxT("PROP %d ") + prop, ev.GetId());
  wxPGMultiButton* buttons = (wxPGMultiButton*) GetGrid()->GetEditorControlSecondary();
  if( ev.GetId() == buttons->GetButtonId(0) )
    wxLogDebug(wxT("0"));
  if( ev.GetId() == buttons->GetButtonId(1) )
    wxLogDebug(wxT("1"));
//  if( ev.GetId() == buttons->GetButtonId(2) )
//    wxLogDebug(wxT("2"));
  if( ev.GetId() == wxPG_SUBID2 )
    wxLogDebug(wxT("grr"));

}

void Q4MAXVisibilityPropertiesCtrl::OnItemChanged( wxPropertyGridEvent& ev )
{
  PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
  if( !p )
  {
    wxLogDebug(wxT("Q4MAXVisibilityPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }

  Q4MAXElement *el = static_cast<Q4MAXElement*>(p->curel());
  wxString name = ev.GetPropertyName();
  wxVariant val = ev.GetPropertyValue();
  if( name == wxT("overwrite-pos") )
  {
    el->add_has( E_HAS_POS, val.GetBool() );
    Vec2 p = el->iget_pos();
    SetPropertyValue( wxT("X"), p.x );
    SetPropertyValue( wxT("Y"), p.y );
  }
  else if( name == wxT("X") || name == wxT("Y") ) //|| name == wxT("height") || name == wxT("width") )
  {
    if( !(el->m_has & E_HAS_POS) )
    { // user was starting to edit while seeing the inherited values, copy them over
      Vec2 p = el->iget_pos();
      el->m_rect.x = (int)p.x;
      el->m_rect.y = (int)p.y;
      el->add_has(E_HAS_POS);
      SetPropertyValue( wxT("overwrite-pos"), true );
    }
    if( name == wxT("X") )
      el->m_rect.x = ev.GetPropertyValueAsInt();
    else if( name == wxT("Y") )
      el->m_rect.y = ev.GetPropertyValueAsInt();
  }
  else if( name == wxT("overwrite-dim") )
  {
    el->add_has( E_HAS_DIM, val.GetBool() );
    Vec2 p = el->iget_dim();
    SetPropertyValue( wxT("width"), p.x );
    SetPropertyValue( wxT("height"), p.y );
  }
  else if( name == wxT("width") || name == wxT("height") )
  {
    if( !(el->m_has & E_HAS_DIM) )
    { // user was starting to edit while seeing the inherited values, copy them over
      Vec2 p = el->iget_dim();
      el->m_rect.width = (int)p.x;
      el->m_rect.height = (int)p.y;
      el->add_has(E_HAS_DIM);
      SetPropertyValue( wxT("overwrite-dim"), true );
    }
    if( name == wxT("width") )
      el->m_rect.width = ev.GetPropertyValueAsInt();
    else if( name == wxT("Y") )
      el->m_rect.height = ev.GetPropertyValueAsInt();
  }
  else if( name == wxT("time") )
  {
    el->add_has( Q4MAX_E_HAS_TIME, !ev.GetProperty()->IsValueUnspecified() );
    if( !ev.GetProperty()->IsValueUnspecified() )
      el->set_ival(name, val.GetInteger());
    SetPropertyValue( name, el->iget_ival(name) );
  }
  else if( name == wxT("visible-forcehidden")
    || name == wxT("visible-duel") || name == wxT("visible-tdm") || name == wxT("visible-ctf") )
  {
    
    bool fh = GetPropertyValueAsBool(wxT("visible-forcehidden"));
    if( name == wxT("visible-forcehidden") )
    {
      el->add_has( Q4MAX_E_HAS_VISIBLE, val.GetBool() );
      if( fh = val.GetBool() )
      { // enabled it
        el->set_ival(wxT("visible"), 0);
      }
      int v = el->iget_ival(wxT("visible"));
      SetPropertyValue(wxT("visible-duel"), !fh && (v & Q4MAX_E_VIS_DUEL) != 0);
      SetPropertyValue(wxT("visible-tdm"), !fh && (v & Q4MAX_E_VIS_TDM) != 0);
      SetPropertyValue(wxT("visible-ctf"), !fh && (v & Q4MAX_E_VIS_CTF) != 0);
    }
    else
    {
      bool vd = GetPropertyValueAsBool(wxT("visible-duel"));
      bool vt = GetPropertyValueAsBool(wxT("visible-tdm"));
      bool vc = GetPropertyValueAsBool(wxT("visible-ctf"));
      int v = 0;
      if( vd || vt || vc )
      {
        el->add_has( Q4MAX_E_HAS_VISIBLE, true );
        SetPropertyValue(wxT("visible-forcehidden"), false);
        if( vd ) v |= Q4MAX_E_VIS_DUEL;
        if( vt ) v |= Q4MAX_E_VIS_TDM;
        if( vc ) v |= Q4MAX_E_VIS_CTF;
      }
      el->set_ival(wxT("visible"), v);
    }
  }
  else
    return; // nothing changed


  wxGetApp().hudfile()->set_modified();
  update_layout();

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}
void Q4MAXVisibilityPropertiesCtrl::from_element( const ElementBase *el )
{
  const Q4MAXElement *cel = static_cast<const Q4MAXElement*>(el);

  SetPropertyValue( wxT("overwrite-pos"), el->has() & E_HAS_POS );
  SetPropertyValue( wxT("overwrite-dim"), el->has() & E_HAS_DIM );

  wxToolBar *tb = GetToolBar();
  tb->ToggleTool( (el->is_enabled() ? ID_BTN_ELEMENT_ENABLE : ID_BTN_ELEMENT_DISABLE), true );
  tb->EnableTool( ID_BTN_ELEMENT_ENABLE,  (el->flags() & E_ENABLEALWAYS)==0 );
  tb->EnableTool( ID_BTN_ELEMENT_DISABLE,  (el->flags() & E_ENABLEALWAYS)==0 );
    
  Vec2 p = el->iget_pos();
  SetPropertyValue( wxT("X"), p.x );
  SetPropertyValue( wxT("Y"), p.y );
  Vec2 d = el->iget_dim();
  SetPropertyValue( wxT("width"), d.x );
  SetPropertyValue( wxT("height"), d.y );

  SetPropertyValue( wxT("time"), cel->iget_ival(wxT("time")) );

  update_layout();
}

void Q4MAXVisibilityPropertiesCtrl::update_layout()
{
  PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
  if( !p )
  {
    wxLogDebug(wxT("Q4MAXVisibilityPropertiesCtrl::OnItemChanged() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  ElementBase *el = p->curel();

  property_defines( wxT("visible-forcehidden"), (el->has() & Q4MAX_E_HAS_VISIBLE) != 0 );
  property_defines( wxT("visible-duel"), (el->has() & Q4MAX_E_HAS_VISIBLE) != 0 );
  property_defines( wxT("visible-tdm"), (el->has() & Q4MAX_E_HAS_VISIBLE) != 0 );
  property_defines( wxT("visible-ctf"), (el->has() & Q4MAX_E_HAS_VISIBLE) != 0 );

  property_defines( wxT("X"), (el->has() & E_HAS_POS)!=0 );
  property_defines( wxT("Y"), (el->has() & E_HAS_POS)!=0 );
  property_defines( wxT("width"), (el->has() & E_HAS_DIM)!=0 );
  property_defines( wxT("height"), (el->has() & E_HAS_DIM)!=0 );

  property_defines(wxT("time"), (el->has() & Q4MAX_E_HAS_TIME) != 0 );
}

void Q4MAXVisibilityPropertiesCtrl::OnElementVisibility( wxCommandEvent& ev )
{
  PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
  if( !p )
  {
    wxLogDebug(wxT("Q4MAXVisibilityPropertiesCtrl::OnElementVisibility() - PropertiesCtrl is not yet available but user shouldn't trigger this function"));
    return;
  }
  ElementBase *el = p->curel();
  el->set_enabled( ev.GetId() == ID_BTN_ELEMENT_ENABLE );

  // -- update elementicon in list
  wxGetApp().mainframe()->elementsctrl()->update_item(el);

  wxGetApp().mainframe()->OnPropertiesChanged();
}

void Q4MAXVisibilityPropertiesCtrl::OnAlign( wxCommandEvent& ev )
{
  Q4MAXElement *el = current_element();
  el->add_has( E_HAS_RECT );
  Vec2 p = el->iget_pos();
  Vec2 d = el->iget_dim();
  switch( ev.GetId() )
  {
  case ID_BTN_ALIGN_LEFT: p.x = 0; break;
  case ID_BTN_ALIGN_CENTER: p.x = (640-d.x)/2; break;
  case ID_BTN_ALIGN_RIGHT: p.x = 640-d.x; break;
  case ID_BTN_ALIGN_TOP: p.y = 0; break;
  case ID_BTN_ALIGN_MIDDLE: p.y = (480-d.y)/2; break;
  case ID_BTN_ALIGN_BOTTOM: p.y = 480-d.y; break;
  default:
    return;
  }
  el->set_pos(p);
  el->set_dim(d);

  // -- propagate
  from_element(el); // update propertygrid
  wxGetApp().mainframe()->OnPropertiesChanged();
}
