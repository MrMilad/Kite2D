#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlist>
#include <qhash>
#include <qstringlist.h>
#include <resourcetree.h>
#include <objecttree.h>
#include <componenttree.h>
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

struct Project {
	QString name;
	QString Path;
	QString resPath;
};

struct ComBinder {
	QString cname;
	QString pname;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void run();

private slots:
void closeEvent(QCloseEvent *event) override;

void newProject();
void openProject();
void saveProject();
void closeProject();

void exitApp();

private:
	void setupActions();
    void setupMenus();
    void setupToolbar();
    void setupDocks();
    void setupScene();
	void setupStatusBar();

	void scanKiteMeta();
	void setupResources();

	void loadDockState();
	void saveDockState();

	void disGUI();
	void enGUI();

	void registerResCallbacks();

	void saveXML(QIODevice *device);
	void loadXML(QIODevice *device);


    QDockWidget *resDock;
    QDockWidget *objDock;
    QDockWidget *prpDock;
    QGraphicsView *sceneView;
	ComponentTree *propTree;
    ResourceTree *resTree;
	ObjectTree *objTree;

	QMenu *fileMenu;
	QMenu *winMenu;
	QMenu *compMenu;

    QToolBar *fileTolb;
	QAction *newProj;
	QAction *openProj;
	QAction *saveProj;
	QAction *closeProj;
	QAction *playScene;

	QAction *exit;

	Kite::KMetaManager *kmman;
	Kite::KResourceManager *krman;

	typedef void(MainWindow::*resEditCallb)(Kite::KResource *);
	typedef void(MainWindow::*resRemoveCallb)(Kite::KResource *);
	struct ResourceCallbacks {
		resEditCallb editCallback;
		resRemoveCallb removeCallb;

		ResourceCallbacks() :
			editCallback(nullptr), removeCallb(nullptr) {}
	};

	QList<QString> kresCatList;
	QList<QString> kcompList;
	QHash<QString, QHash<QString, Kite::KResource *>> kresMap;
	QHash<QString, QString> kresDict;
	QHash<QString, ResourceCallbacks> kresCallbackMap;

	Project *curProject;
	Kite::KScene *kcurScene;
	QTreeWidgetItem *curEnt;
	QString curCom;
};

#endif // MAINWINDOW_H
