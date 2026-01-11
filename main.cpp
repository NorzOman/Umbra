
#include <QApplication>
#include "src/umbra.h"

int main(int argc , char* argv[]){
    QApplication app(argc,argv);

    // -- main umbra --
    Umbra u;
    u.show();

    return app.exec();
}
