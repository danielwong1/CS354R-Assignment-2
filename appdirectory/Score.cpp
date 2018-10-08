#include "Score.h"

Score::Score()
{
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "Score/Sheet");

    CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "Score/Button");
    quit->setText("Score\n3");
    quit->setSize(CEGUI::USize(CEGUI::UDim(0.08, 0), CEGUI::UDim(0.10, 0)));

    sheet->addChild(quit);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}
