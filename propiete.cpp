#include "propiete.h"
#include "ui_propiete.h"

Propiete::Propiete(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Propiete)
{
    ui->setupUi(this);
//    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
//                                        | QDialogButtonBox::Cancel);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

int Propiete::getWidth(){
    return ui->LargeurNumber->value();
}

int Propiete::getHeight(){
    return ui->HauteurNumber->value();
}

Propiete::~Propiete()
{
    delete ui;
}

