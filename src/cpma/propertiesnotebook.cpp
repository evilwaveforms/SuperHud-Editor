#include "propertiesnotebook.h"

#include "visibilitypropertiesctrl.h"
#include "fontpropertiesctrl.h"
#include "colorpropertiesctrl.h"
#include "miscpropertiesctrl.h"

#include "element.h"
#include <wx/propgrid/propgrid.h>



CPMAPropertiesNotebook::CPMAPropertiesNotebook( wxWindow *parent ) : 
  PropertiesNotebookBase(parent)
{
  wxPropertyGrid::SetBoolChoices(_("On"), _("Off"));

  m_vis = new VisibilityPropertiesCtrl(this);
  m_font = new FontPropertiesCtrl(this);
  m_color = new ColorPropertiesCtrl(this);
  m_misc = new MiscPropertiesCtrl(this);
  AddPage( m_vis, _("Visibility") );
  AddPage( m_font, _("Font") );
  AddPage( m_color, _("Color") );
  AddPage( m_misc, _("Misc") );
}

void CPMAPropertiesNotebook::update_from_element( const elements_type& els )
{
  if( els.size() != 1 )
  { // no properties selection
    
    // CollapseAll/ExpandAll trigger EVT_PG_CHANGING/CHANGED if we have been editing
    // a value that wasn't yet saved. so we have to make sure that we set
    // m_curel _afterwards_
    m_vis->CollapseAll();
    m_font->CollapseAll();
    m_color->CollapseAll();
    m_misc->CollapseAll();
    m_vis->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_ENABLE, false );
    m_vis->GetToolBar()->ToggleTool( ID_BTN_ELEMENT_DISABLE, false );
    Disable();
    m_curel = 0;
  }
  else
  {
    Enable();
    m_vis->ExpandAll();
    m_font->ExpandAll();
    m_color->ExpandAll();
    m_misc->ExpandAll();
    m_curel = els.front();
    m_vis->from_element(m_curel);
    m_font->from_element(m_curel);
    m_color->from_element(m_curel);
    m_misc->from_element(m_curel);
    m_vis->ClearModifiedStatus();
    m_font->ClearModifiedStatus();
    m_color->ClearModifiedStatus();
    m_misc->ClearModifiedStatus();
  }
}



