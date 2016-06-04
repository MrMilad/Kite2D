#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlist>
#include <qhash>
#include <qstringlist.h>
#include <kiteinfo.h>
#include <resourcedock.h>
#include <objectdock.h>
#include <componentdock.h>
#include <outputdock.h>
#include <maintab.h>
#include "executer.h"
#include <Kite/engine/kengine.h>

class QGraphicsView;
class QMenu;
class QTabWidget;
class QToolBox;
class QStatusBar;
class QAction;

struct Project {
	QString name;
	QString Path;
	QString resPath;
	Kite::KConfig config;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
void showSwitchOutput();
void closeEvent(QCloseEvent *event) override;

void startEngine();
void getEngineOutput(const QString &Text, int MType);

void engineStarted();
void enginePaused();
void engineUnpaused();
void engineStoped();

void newProject();
void openProject();
void saveProject();
void closeProject();
void openProjSetting();
void exitApp();

private:
	void setupActions();
    void setupMenus();
    void setupToolbar();
    void setupDocks();
    void setupScene();
	void setupStatusBar();

	void loadDockState();
	void saveDockState();

	void disGUI();
	void enGUI();

	void saveXML(QIODevice *device, const QString &Address);
	bool loadXML(QIODevice *device, const QString &Address);

    ResourceDock *resDock;
    ObjectDock *objDock;
    ComponentDock *prpDock;
	QDockWidget *expDock;
	OutputDock *outDock;

	MainTab *mainTab;
    QGraphicsView *sceneView;

	QMenu *fileMenu;
	QMenu *winMenu;
	QMenu *compMenu;

    QToolBar *fileTolb;
	QAction *newProj;
	QAction *openProj;
	QAction *saveProj;
	QAction *closeProj;
	QAction *playScene;
	QAction *pauseScene;
	QAction *stopScene;
	QAction *projSettings;
	QAction *showOutputPan;
	QAction *exit;

	QHash<QString, QWidget *> resTabs;

	Project *curProject;
	KiteInfo *kinfo;
	Executer *exec;
};

#endif // MAINWINDOW_H
