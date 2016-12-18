#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qhash>
#include <Kite/engine/kengine.h>

class QGraphicsView;
class QMenu;
class QTabWidget;
class QToolBox;
class QStatusBar;
class QAction;
class QActionGroup;
class QUndoGroup;

class ObjectDock;
class ComponentDock;
class QDockWidget;
class OutputDock;
class ResourceDock;
class KiteInfo;
class Executer;
class MainTab;

struct Project {
	QString name;
	QString Path;
	Kite::KConfig config;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
void toggleFullScreen();
void toggleDesignView();

void aboutDialogue();
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
void applyChanges();
void closeProject(bool Silent = false);
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

	void saveXML();
	bool loadXML(QIODevice *device, const QString &Address);

	void initProject();
	void clearProject();

	QUndoGroup *undoGroup;

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
	
	QAction *actUndo;
	QAction *actRedo;
	QAction *actNew;
	QAction *actOpen;
	QAction *actSave;
	QAction *actClose;
	QAction *actPlay;
	QAction *actPause;
	QAction *actStop;
	QAction *actSetting;
	QAction *actShowOut;
	QAction *actExit;

	QHash<QString, QWidget *> resTabs;

	Project *curProject;
	KiteInfo *kinfo;
	Executer *exec;
};

#endif // MAINWINDOW_H
