#include "Score.h"
#include <string>
#include <sstream>


Score::Score()
{
    score = 0;
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "Score/Sheet");

    scoreWindow = wmgr.createWindow("TaharezLook/Button", "Score/Button");
    std::stringstream scoreString;
	scoreString << "Score\n " << score;
    scoreWindow->setText(scoreString.str());

    scoreWindow->setSize(CEGUI::USize(CEGUI::UDim(0.08, 0), CEGUI::UDim(0.10, 0)));

    sheet->addChild(scoreWindow);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

void Score::setScore(int count) {
    score = count;
    std::stringstream scoreString;
	scoreString << "Score\n " << score;
    scoreWindow->setText(scoreString.str());
}