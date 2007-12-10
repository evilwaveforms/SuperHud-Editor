#ifndef __FONTPROPERTIESCTRL_H__
#define __FONTPROPERTIESCTRL_H__

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>


class ElementBase;

class FontPropertiesCtrl : public wxPropertyGridManager
{
  public:
    FontPropertiesCtrl( wxWindow *parent );

    void from_element( ElementBase *el );
    void update_layout();

  private:
    void OnItemChanged( wxPropertyGridEvent& ev );


    DECLARE_EVENT_TABLE()
};

#endif // __FONTPROPERTIESCTRL_H__
