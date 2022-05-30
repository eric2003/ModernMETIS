#include "metis.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

typedef std::vector<idx_t> vidx_t;


int main(int argc, char *argv[])
{
    namespace fs = std::filesystem;

    std::cout << "Current working directory: " << fs::current_path() << '\n';

    std::fstream file;
    file.open("../example_a.graph");
    std::string line;
    std::getline(file, line);
    int nvtxs = -1;
    int nedges = -1;
    int fmt = 0;
    int ncon = -1;
    char fmtstr[256];

    int nfields = std::sscanf(line.c_str(), "%d %d %d %d", &nvtxs, &nedges, &fmt, &ncon);

    std::sprintf(fmtstr, "%03d", fmt % 1000);
    int readvs = (fmtstr[0] == '1');
    int readvw = (fmtstr[1] == '1');
    int readew = (fmtstr[2] == '1');

    int nedges2 = nedges * 2;

    vidx_t xadj, adjncy, vwgt, adjwgt, vsize;
    vidx_t cellzone;
    xadj.resize(nvtxs + 1);
    adjncy.resize(2 * nedges);
    adjwgt.resize(2 * nedges);

    xadj[0] = 0;
    int k = 0;

    for ( int i = 0; i < nvtxs; ++ i )
    {
        std::getline(file, line);

        // Read vertex sizes
        if ( readvs ) {
        }

        // Read vertex weights
        if ( readvw ) {
        }

        std::size_t pos{};

        while ( true ) {
            int edge = std::stoi(line, &pos);
            line = line.substr(pos);
            if ( line == "" )
                break; /* End of line */
 
            if (edge < 1 || edge > nvtxs)
            {
                ;
            }

            int ewgt = 1;
            if ( readew ) {
                ewgt = std::stoi(line, &pos);
            }

            if ( k == nedges2 )
            {
                exit(0);
            }

            adjncy[k] = edge - 1;
            adjwgt[k] = ewgt;
            k++;
        }
        xadj[i + 1] = k;
    }

    file.close();
    return 0;
}