#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlist>
#include <qhash>
#include <qstringlist.h>
#include <kiteinfo.h>
#include <resourcetree.h>
#include <objecttree.h>
#include <componenttree.h>
#include <maintab.h>
#include "executer.h"
#include <Kite/engine/kengine.h>

class QGraphicsView;
class QMenu;
class QDockWidget;
class QTreeWidget;
class QTreeWidgetItem;
class QTabWidget;
class QTableWidget;
class QToolBox;
class QStatusBar;
class QListWidget;
class QLineEdit;
class QAction;
class QFormLayout;
class QFrame;
class QCheckBox;
class QTextEdit;

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
void closeEvent(QCloseEvent *event) override;

void startEngine();

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

    QDockWidget *resDock;
    QDockWidget *objDock;
    QDockWidget *prpDock;
	QDockWidget *expDock;
	QDockWidget *outDock;
	MainTab *mainTab;
    QGraphicsView *sceneView;
	ComponentTree *propTree;
    ResourceTree *resTree;
	ObjectTree *objTree;
	QTextEdit *koutput;

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
	QAction *exit;

	QHash<QString, QWidget *> resTabs;

	Project *curProject;
	KiteInfo *kinfo;
	Executer *exec;
};

#endif // MAINWINDOW_H
