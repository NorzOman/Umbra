
#include <QApplication>
#include "umbra.h"

int main(int argc , char* argv[]){
    QApplication app(argc,argv);
    Umbra u;
    u.show();

    return app.exec();
}
