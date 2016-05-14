#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlist>
#include <qhash>
#include <qstringlist.h>
#include <resourcetree.h>
#include <objecttree.h>
#include <componenttree.h>
#include <maintab.h>
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

	void loadDockState();
	void saveDockState();

	void disGUI();
	void enGUI();

	void saveXML(QIODevice *device, const QString &Address);
	bool loadXML(QIODevice *device, const QString &Address);

    QDockWidget *resDock;
    QDockWidget *objDock;
    QDockWidget *prpDock;
	MainTab *mainTab;
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

	QList<QString> kresCatList;
	QList<QString> kcompList;
	QStandardItemModel *completerModel;
	QHash<QString, QWidget *> resTabs;

	Project *curProject;
};

#endif // MAINWINDOW_H
