#include "vgui/vgui.h"
#include "vgui_controls/Frame.h"

class IGameUI
{
public:
	virtual void Create( vgui::VPANEL parent ) = 0;
	virtual void Destroy( void ) = 0;
	virtual vgui::Panel *GetPanel(void) = 0;
};
#define DeclarePanel(className,panelClassName,globalPanel)\
	class className : public IGameUI\
	{\
	private:\
		panelClassName *myPanel;\
	public:\
		className(void)\
		{\
			myPanel = NULL;\
		}\
		void Create( vgui::VPANEL parent )\
		{\
			myPanel = new panelClassName( parent );\
		}\
		void Destroy( void )\
		{\
			if(myPanel)\
			{\
				myPanel->SetParent( (vgui::Panel *)NULL );\
				delete myPanel;\
			}\
		}\
		vgui::Panel *GetPanel(void)\
		{\
			return myPanel;\
		}\
	};\
	extern IGameUI *globalPanel;

//Doesn't use panelClassName but has it to base code on copy and paste work 
#define PanelGlobals(className,panelClassName,globalPanel)\
	static className g_##className##Panel;\
	IGameUI *globalPanel = (IGameUI *)&g_##className##Panel

#define ToggleVisibility(panel)\
	panel->SetVisible(!panel->IsVisible())

/*//only the prototype for AlignPanel exists 
#define CenterThisPanelOnScreen()\
	int x,w,h;\
	GetBounds(x,x,w,h);\
	SetPos((ScreenWidth()-w)/2,(ScreenHeight()-h)/2)
#define CenterPanelOnScreen(panel)\
	int x,w,h;\
	panel->GetBounds(x,x,w,h);\
	panel->SetPos((panel->ScreenWidth()-w)/2,(panel->ScreenHeight()-h)/2)*/

