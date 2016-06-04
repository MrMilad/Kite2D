#include "kiteinfo.h"
#include <Kite/meta/kmetabase.h>
#include <Kite/meta/kmetaclass.h>
#include <Kite/meta/kmetaenum.h>
#include <Kite/meta/kmetapod.h>
#include <Kite/meta/kmetamanager.h>
#include <Kite/core/kresourcemanager.h>
#include <kmeta.khgen.h>

KiteInfo::KiteInfo():
	model(new QStandardItemModel),
	resources(new QStringList), components(new QStringList)
{
	// register kiet meta system
	Kite::KMetaManager kmman;
	Kite::registerKiteMeta(&kmman);

	// dump all meta information
	std::vector<const Kite::KMetaBase *> meta;
	kmman.dump(meta);

	// kite root namespace
	auto kroot = new QStandardItem(QIcon(":/icons/dia16"), "Kite");
	kroot->setToolTip("<font color = \"orange\">Kite2D</font>");
	model->appendRow(kroot);

	// searching for resource categories
	for (auto it = meta.begin(); it != meta.end(); ++it) {
		// Resorces
		if (((*it)->getFlag() & RESOURCE) && !((*it)->getFlag() & ABSTRACT)) {
			resources->push_back((*it)->getName().c_str());
			auto item = new QStandardItem(QIcon(":/icons/res16"), (*it)->getName().c_str());
			item->setToolTip("<font color = \"orange\">resource</font>");
			kroot->appendRow(item);
		}

		// Components
		if (((*it)->getFlag() & COMPONENT) && !((*it)->getFlag() & ABSTRACT)) {
			auto comp = (Kite::KMetaClass *)(*it);
			auto infoList = (*it)->getInfo();
			auto propList = comp->getProperties();
			auto funList = comp->getFunctions();
			for (auto ilit = infoList->begin(); ilit != infoList->end(); ++ilit) {
				if (ilit->first == "KI_CTYPE") {
					components->push_back(ilit->second.c_str());
					auto item = new QStandardItem(QIcon(":/icons/comp16"), ilit->second.c_str());
					item->setToolTip("<font color = \"orange\">component</font><br>" + QString::number(propList->size()) + " properties");
					kroot->appendRow(item);

					// component properties
					for (auto piit = propList->begin(); piit != propList->end(); ++piit) {
						auto pitem = new QStandardItem();
						pitem->setText(piit->name.c_str());
						QString tip("<font color = \"orange\">property</font><br>"
									"<font color = \"green\">comment: </font>");
						tip += piit->comment.c_str();
						if (piit->type == Kite::KMetaPropertyTypes::KMP_BOTH) {
							pitem->setIcon(QIcon(":/icons/prop16"));
							tip += "<br><font color = \"red\">access type: </font>read/write<br><font color = \"#00a4f0\">type: </font>";
						} else {
							tip += "<br><font color = \"red\">access type: </font>read-only<br><font color = \"#00a4f0\">type: </font>";
							pitem->setIcon(QIcon(":/icons/propro16"));
						}
						pitem->setToolTip(tip + QString(piit->typeName.c_str()));
						item->appendRow(pitem);

						// is resource field
						if (!piit->resType.empty()) {
							resComponents[ilit->second.c_str()].push_back(*piit);
						}
					}

					// component functions
					for (auto fit = funList->begin(); fit != funList->end(); ++fit) {
						auto fitem = new QStandardItem();
						fitem->setText(fit->name.c_str());
						fitem->setToolTip("<font color = \"orange\">function</font>");
						fitem->setIcon(QIcon(":/icons/fun16"));
						item->appendRow(fitem);
					}

					break;
				}
			}
		}

		// Enum
		if ((*it)->getMetaType() == Kite::KMetaTypes::KMT_ENUM) {
			auto kenum = (Kite::KMetaEnum *)(*it);
			auto members = kenum->getMembers();
			QString ename((*it)->getName().c_str());
			auto bitem = new QStandardItem(QIcon(":/icons/benm16"), ename);
			QString btip("<font color = \"orange\">enum</font><br>" +
						 QString::number(members->size()) + " members");

			bitem->setToolTip(btip);
			kroot->appendRow(bitem);

			for (auto eit = members->begin(); eit != members->end(); ++eit) {
				QString tip("<font color = \"orange\">enum member</font><br>"
							"<font color = \"#00a4f0\">base type: </font>" + ename + "<br>"
							"<font color = \"#00a4f0\">value: </font>" + QString::number(eit->value));
				auto item = new QStandardItem();
				item->setIcon(QIcon(":/icons/enm16"));
				item->setText(eit->name.c_str());
				item->setToolTip(tip);
				bitem->appendRow(item);
				//model->appendRow(item);
			}
		}

		// POD
		if ((*it)->getMetaType() == Kite::KMetaTypes::KMT_CLASS && ((*it)->getFlag() & POD)) {
			kroot->appendRow(new QStandardItem(QIcon(":/icons/pod16"), (*it)->getName().c_str()));
		}

		// Scriptables
		if ((*it)->getMetaType() == Kite::KMetaTypes::KMT_CLASS &&
			((*it)->getFlag() & SCRIPTABLE) &&
			!((*it)->getFlag() & ABSTRACT)) {
			auto cls = (Kite::KMetaClass *)(*it);
			QString cname(cls->getName().c_str());

			// functions\properties
			auto funList = cls->getFunctions();
			auto propList = cls->getProperties();

			auto bitem = new QStandardItem(QIcon(":/icons/enum"), cname);
			QString btip("<font color = \"orange\">class</font><br>" +
						 QString::number(funList->size()) + " functions<br>" +
						 QString::number(propList->size()) + " properties");

			bitem->setIcon(QIcon(":/icons/cls16"));
			bitem->setText(cname);
			bitem->setToolTip(btip);
			kroot->appendRow(bitem);

			// functions 
			for (auto fit = funList->begin(); fit != funList->end(); ++fit) {
				if (fit->isStatic) {
					bitem->appendRow(new QStandardItem(QIcon(":/icons/fun16"), cname + "." + fit->name.c_str()));
				} else {
					bitem->appendRow(new QStandardItem(QIcon(":/icons/fun16"), fit->name.c_str()));
				}
			}

			// properties
			for (auto pit = propList->begin(); pit != propList->end(); ++pit) {
				if (pit->type == Kite::KMetaPropertyTypes::KMP_BOTH) {
					bitem->appendRow(new QStandardItem(QIcon(":/icons/prop16"), pit->name.c_str()));
				} else {
					bitem->appendRow(new QStandardItem(QIcon(":/icons/propro16"), pit->name.c_str()));
				}
			}
		}

	}
}

KiteInfo::~KiteInfo() {
	delete model;
	delete resources;
	delete components;
}