#include "frmexeorder.h"
#include "qpushbutton.h"
#include "ui_frmexeorder.h"
#include <vector>

frmexeorder::frmexeorder(Kite::KEntity *Entity, QWidget *parent) :
	currEnt(Entity),
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::frmexeorder)
{
    ui->setupUi(this);
	setWindowTitle("Logic Execution Order");

	ui->lstScripts->setDragEnabled(true);
	ui->lstScripts->setAcceptDrops(true);
	ui->lstScripts->setDragDropMode(QAbstractItemView::DragDropMode::InternalMove);
	ui->lstScripts->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);

	std::vector<Kite::KHandle> list;
	Entity->getScriptComponents(list);

	for (auto it = list.begin(); it != list.end(); ++it) {
		auto comp = Entity->getComponent((*it));
		ui->lstScripts->addItem(comp->getName().c_str());
	}

	connect(ui->btnClose->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &frmexeorder::close);
	connect(ui->lstScripts->model(), &QAbstractItemModel::rowsAboutToBeMoved, this, &frmexeorder::reorder);
}

frmexeorder::~frmexeorder(){
    delete ui;
}

void frmexeorder::reorder(const QModelIndex &sourceParent, int sourceStart, int sourceEnd,
						  const QModelIndex &destinationParent, int destinationRow) {
	
	if (sourceStart < destinationRow) --destinationRow;
	auto comp = currEnt->getComponentByName(Kite::CTypes::Logic, ui->lstScripts->item(sourceStart)->text().toStdString());
	currEnt->reorderScriptComponent(comp->getHandle(), destinationRow);
}
