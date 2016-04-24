#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlist>
#include <qhash>
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
class QAction;

struct Project {
	QString name;
	QString Path;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void run();

private slots:
void closeEvent(QCloseEvent *event);

void newProject();
void openProject();
void saveProject();
void closeProject();

void resourceClicked();
void resourceRClicked(const QPoint & pos);
void resourceAdd();
void resourceOpen();
void resourceSave();
void resourceEdit();
void resourceRemove();

void entityClicked();
void entityRClicked(const QPoint & pos);
void entityAdd();
void entityRemove();
void entityRename();

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

	// KScene
	void editScene(Kite::KResource *Scene);
	void removeScene(Kite::KResource *Scene);
	void loadChilds(Kite::KEntityManager *Eman, const Kite::KHandle &Entity, QTreeWidgetItem *Parrent);

    QDockWidget *resDock;
    QDockWidget *objDock;
    QDockWidget *prpDock;
    QGraphicsView *sceneView;
	QTreeWidget *propTree;
    QTreeWidget *resTree;
	QTreeWidget *objTree;

	QMenu *fileMenu;
	QMenu *winMenu;

    QToolBar *fileTolb;
	QAction *newProj;
	QAction *openProj;
	QAction *saveProj;
	QAction *closeProj;
	QAction *playScene;
	QAction *addRes;
	QAction *openRes;
	QAction *saveRes;
	QAction *editRes;
	QAction *remRes;
	QAction *addObj;
	QAction *remObj;
	QAction *renObj;
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
	QHash<QTreeWidgetItem *, QHash<QString, Kite::KResource *>> kresMap;
	QHash<QString, ResourceCallbacks> kresCallbackMap;

	Project *curProject;
	Kite::KScene *kcurScene;
	QTreeWidgetItem *curEnt;
};

#endif // MAINWINDOW_H
