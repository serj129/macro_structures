/********************************************************************************
** Form generated from reading UI file 'struct.ui'
**
** Created: Thu 28. Apr 11:45:07 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STRUCT_H
#define UI_STRUCT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_structure
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label;
    QLineEdit *lineEdit;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QPushButton *colorButton;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QComboBox *comboBox_3;
    QPushButton *clearButton;
    QPushButton *zoomOutButton;
    QPushButton *zoomInButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QWidget *structure)
    {
        if (structure->objectName().isEmpty())
            structure->setObjectName(QString::fromUtf8("structure"));
        structure->setWindowModality(Qt::ApplicationModal);
        structure->resize(726, 480);
        gridLayoutWidget = new QWidget(structure);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(350, 150, 160, 80));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(structure);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(250, 10, 28, 16));
        lineEdit_2 = new QLineEdit(structure);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(280, 10, 133, 20));
        label = new QLabel(structure);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(170, 10, 16, 16));
        lineEdit = new QLineEdit(structure);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(180, 10, 61, 20));
        groupBox = new QGroupBox(structure);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(11, 11, 97, 90));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setChecked(true);

        verticalLayout->addWidget(radioButton);

        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        verticalLayout->addWidget(radioButton_2);

        colorButton = new QPushButton(groupBox);
        colorButton->setObjectName(QString::fromUtf8("colorButton"));

        verticalLayout->addWidget(colorButton);

        comboBox = new QComboBox(structure);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(11, 107, 140, 20));
        comboBox_2 = new QComboBox(structure);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(11, 133, 141, 20));
        comboBox_3 = new QComboBox(structure);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));
        comboBox_3->setGeometry(QRect(11, 159, 137, 20));
        clearButton = new QPushButton(structure);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setGeometry(QRect(11, 231, 75, 23));
        zoomOutButton = new QPushButton(structure);
        zoomOutButton->setObjectName(QString::fromUtf8("zoomOutButton"));
        zoomOutButton->setGeometry(QRect(11, 260, 75, 23));
        zoomInButton = new QPushButton(structure);
        zoomInButton->setObjectName(QString::fromUtf8("zoomInButton"));
        zoomInButton->setGeometry(QRect(11, 289, 75, 23));
        saveButton = new QPushButton(structure);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(11, 318, 75, 23));
        cancelButton = new QPushButton(structure);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(11, 356, 75, 23));
        okButton = new QPushButton(structure);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(11, 385, 75, 23));

        retranslateUi(structure);
        QObject::connect(cancelButton, SIGNAL(clicked()), structure, SLOT(close()));

        QMetaObject::connectSlotsByName(structure);
    } // setupUi

    void retranslateUi(QWidget *structure)
    {
        structure->setWindowTitle(QApplication::translate("structure", "Edit structure", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("structure", "lamda", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setText(QApplication::translate("structure", "0.2", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("structure", "A", 0, QApplication::UnicodeUTF8));
        lineEdit->setText(QApplication::translate("structure", "0.1", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("structure", "Edit structure", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("structure", "Structure 1", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("structure", "Structure 2", 0, QApplication::UnicodeUTF8));
        colorButton->setText(QApplication::translate("structure", "Color", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("structure", "Potential for structure 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("structure", "Coulomb potential", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("structure", "Debye-H\303\274ckel potential", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("structure", "Dipole-dipole potential", 0, QApplication::UnicodeUTF8)
        );
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("structure", "Potential for structure 2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("structure", "Coulomb potential", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("structure", "Debye-H\303\274ckel potential", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("structure", "Dipole-dipole potential", 0, QApplication::UnicodeUTF8)
        );
        comboBox_3->clear();
        comboBox_3->insertItems(0, QStringList()
         << QApplication::translate("structure", "Potential for 1-2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("structure", "Coulomb potential", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("structure", "Debye-H\303\274ckel potential", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("structure", "Dipole-dipole potential", 0, QApplication::UnicodeUTF8)
        );
        clearButton->setText(QApplication::translate("structure", "Clear", 0, QApplication::UnicodeUTF8));
        zoomOutButton->setText(QApplication::translate("structure", "Zoom Out", 0, QApplication::UnicodeUTF8));
        zoomInButton->setText(QApplication::translate("structure", "Zoom In", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("structure", "Save", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("structure", "Cancel", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("structure", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class structure: public Ui_structure {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STRUCT_H
