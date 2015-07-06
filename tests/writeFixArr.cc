//    (c) University of Western Australia
//    International Centre of Radio Astronomy Research
//    M468, 35 Stirling Hwy
//    Crawley, Perth WA 6009
//    Australia
//
//    This library is free software: you can redistribute it and/or
//    modify it under the terms of the GNU General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this library. If not, see <http://www.gnu.org/licenses/>.
//
//    Any bugs, questions, concerns and/or suggestions please email to
//    jason.wang@icrar.org

#include "../AdiosStMan.h"
#include <casacore/tables/Tables/SetupNewTab.h>
#include <casacore/tables/Tables/ScaColDesc.h>
#include <casacore/tables/Tables/ArrColDesc.h>
#include <casacore/tables/Tables/ArrayColumn.h>
#include <casacore/tables/DataMan/TiledShapeStMan.h>
#include <casacore/casa/namespace.h>

int main(int argc, char **argv){

    if (argc < 2){
        cout << "./wAdiosStMan /path/to/file" << endl;
        return -1;
    }
    string filename = argv[1];

    IPosition data_pos = IPosition(2,6,5);

    int NrRows = 4;
    Array<float> data_arr(data_pos);
    indgen (data_arr);

    // define a storage manager
//    AdiosStMan stman("MPI", "", 100);
//    AdiosStMan stman("MPI_AGGREGATE", "num_aggregators=32", 100);
    AdiosStMan stman;
//    TiledShapeStMan stman("Ti", data_pos);

    // define a table description & add a scalar column and an array column
    TableDesc td("", "1", TableDesc::Scratch);
    td.addColumn (ArrayColumnDesc<float>("data", -1, ColumnDesc::FixedShape));

    // create a table instance, bind it to the storage manager & allocate rows
    SetupNewTable newtab(filename, td, Table::New);
    newtab.bindAll(stman);
    newtab.setShapeColumn("data", data_pos);
    Table tab(newtab, NrRows);

    // define column objects and link them to the table
    ArrayColumn<float> data_col (tab, "data");

    // write data into the column objects
    for (uInt i=0; i<NrRows; i++) {
        data_col.put(i, data_arr);
    }

    return 0;
}

