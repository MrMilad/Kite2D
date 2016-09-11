#include "glsleditor.h"
#include "qboxlayout.h"
#include <qabstractitemview.h>

Completer *GLSLEditor::completer = new Completer();
GLSLEditor::GLSLEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent):
    TabWidget(Res, KInfo, Parent),
    shader((Kite::KShader *)Res),
    editor(nullptr)
{}

GLSLEditor::~GLSLEditor(){
    delete hlight;
    delete editor;
}

void GLSLEditor::inite() {
	// inite kite2D model
	initeModel();

	// create editor
	editor = new CodeEditor(completer, this);

	// create highlighter
	hlight = new GLSLHLight(editor->document());

	// main layout
	auto mlayout = new QVBoxLayout(this);
	mlayout->setMargin(0);
	mlayout->setSpacing(0);
	mlayout->addWidget(editor);

	// set editor text
	editor->clear();
	if (shader->getCode().empty()) {
		if (shader->getShaderType() == Kite::ShaderType::VERTEX) {
			editor->setPlainText("#version 330\n"
								 "attribute vec2 in_pos;\n"
								 "attribute vec2 in_uv;\n"
								 "attribute vec4 in_col;\n"
								 "out vec4 ex_col;\n"
								 "out vec2 ex_uv;\n"
								 "void main(void) {\n\t\n}");

		} else if (shader->getShaderType() == Kite::ShaderType::FRAGMENT) {
			editor->setPlainText("#version 330\n"
								 "in vec4 ex_col;\n"
								 "in vec2 ex_uv;\n"
								 "uniform sampler2D in_texture;\n"
								 "out vec4 out_col;\n"
								 "void main(void) {\n\t\n}");
		}
	} else {
		editor->appendPlainText(shader->getCode().c_str());
	}
	editor->show();

	// connect completer
	connect(completer, SIGNAL(activated(QString)),
			editor, SLOT(insertCompletion(QString)));

}

bool GLSLEditor::saveChanges() {
	shader->loadString(editor->document()->toPlainText().toStdString(), shader->getShaderType());
	shader->setModified(true);
	return true;
}

void GLSLEditor::reload() {
	if (shader->getCode().empty()) {
		if (shader->getShaderType() == Kite::ShaderType::VERTEX) {
			editor->setPlainText("#version 330\n"
								 "attribute vec2 in_pos;\n"
								 "attribute vec2 in_uv;\n"
								 "attribute vec4 in_col;\n"
								 "out vec4 ex_col;\n"
								 "out vec2 ex_uv;\n"
								 "void main(void) {\n\t\n}");

		} else if (shader->getShaderType() == Kite::ShaderType::FRAGMENT) {
			editor->setPlainText("#version 330\n"
								 "in vec4 ex_col;\n"
								 "in vec2 ex_uv;\n"
								 "uniform sampler2D in_texture;\n"
								 "out vec4 out_col;\n"
								 "void main(void) {\n\t\n}");
		}
	} else {
		editor->appendPlainText(shader->getCode().c_str());
	}
}

void GLSLEditor::initeModel() {
	static bool inite = false;
	static QStandardItemModel *model = new QStandardItemModel();
	if (!inite) {
		completer->setModel(model);
		completer->popup()->setStyleSheet("QToolTip { border: 1px solid #2c2c2c; background-color: #242424; color: white;}");
		completer->setMaxVisibleItems(9);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		completer->setWrapAround(false);

		// add lua keywords into the model
		QStringList luaKeys;
		luaKeys << "and" << "break" << "do"
			<< "false" << "for" << "const"
			<< "not" << "then" << "true"
			<< "else" << "elseif" << "void"
			<< "goto" << "if" << "return"
			<< "repeat" << "while"
			<< "attribute" << "uniform" << "varying"
			<< "layout" << "centroid" << "flat"
			<< "smooth" << "noperspective" << "patch"
			<< "sample" << "subroutine" << "in"
			<< "out" << "inout" << "invariant"
			<< "discard" << "mat2" << "mat3"
			<< "mat4" << "dmat2" << "dmat3"
			<< "dmat4" << "mat2x2" << "mat2x3"
			<< "mat2x4" << "dmat2x2" << "dmat2x3"
			<< "dmat2x4" << "mat3x2" << "mat3x3"
			<< "mat3x4" << "dmat3x2" << "dmat3x3"
			<< "dmat3x4" << "mat4x2" << "mat4x3"
			<< "mat4x4" << "dmat4x2" << "dmat4x3"
			<< "dmat4x4" << "vec2" << "vec3"
			<< "vec4" << "ivec2" << "ivec3"
			<< "ivec4" << "bvec2" << "bvec3"
			<< "bvec4" << "dvec2" << "dvec3"
			<< "dvec4" << "uvec2" << "uvec3"
			<< "uvec4" << "lowp" << "mediump"
			<< "highp" << "precision" << "sampler1D"
			<< "sampler2D" << "sampler3D" << "samplerCube"
			<< "sampler1DShadow" << "sampler2DShadow" << "samplerCubeShadow"
			<< "sampler1DArray" << "sampler2DArray" << "sampler1DArrayShadow"
			<< "sampler2DArrayShadow" << "isampler1D" << "isampler2D"
			<< "isampler3D" << "isamplerCube" << "isampler1DArray"
			<< "isampler2DArray" << "usampler1D" << "usampler2D"
			<< "usampler3D" << "usamplerCube" << "usampler1DArray"
			<< "usampler2DArray" << "sampler2DRect" << "sampler2DRectShadow"
			<< "isampler2DRect" << "usampler2DRect" << "samplerBuffer"
			<< "isamplerBuffer" << "usamplerBuffer" << "sampler2DMS"
			<< "isampler2DMS" << "usampler2DMS" << "sampler2DMSArray"
			<< "isampler2DMSArray" << "usampler2DMSArray" << "samplerCubeArray"
			<< "samplerCubeArrayShadow" << "isamplerCubeArray" << "usamplerCubeArray";

		for (auto it = luaKeys.begin(); it != luaKeys.end(); ++it) {
			model->appendRow(new QStandardItem(QIcon(":/icons/key16"), (*it)));
		}
		inite = true;
	}
}