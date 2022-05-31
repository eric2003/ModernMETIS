#include "metis.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>

typedef std::vector<idx_t> vidx_t;

void PartitionByMetis(idx_t nCells, vidx_t& xadj, vidx_t& adjncy, int nPart, vidx_t& cellzone)
{
    idx_t   ncon = 1;
    idx_t* vwgt = 0;
    idx_t* vsize = 0;
    idx_t* adjwgt = 0;
    real_t* tpwgts = 0;
    real_t* ubvec = 0;
    idx_t options[METIS_NOPTIONS];
    idx_t objval;
    idx_t nZone = nPart;

    METIS_SetDefaultOptions(options);
    std::cout << "Now begining partition graph!\n";
    if (nZone > 8)
    {
        std::cout << "Using K-way Partitioning!\n";
        METIS_PartGraphKway(&nCells, &ncon, xadj.data(), adjncy.data(), vwgt, vsize, adjwgt,
            &nZone, tpwgts, ubvec, options, &objval, cellzone.data());
    }
    else
    {
        std::cout << "Using Recursive Partitioning!\n";
        METIS_PartGraphRecursive(&nCells, &ncon, xadj.data(), adjncy.data(), vwgt, vsize, adjwgt,
            &nZone, tpwgts, ubvec, options, &objval, cellzone.data());
    }
    std::cout << "The interface number: " << objval << std::endl;
    std::cout << "Partition is finished!\n";
}

void PartitionWeight(idx_t nCells, vidx_t& xadj, vidx_t& adjncy, vidx_t& adjwgt, int nPart, vidx_t& cellzone)
{
    idx_t   ncon = 1;
    idx_t* vwgt = 0;
    idx_t* vsize = 0;
    real_t* tpwgts = 0;
    real_t* ubvec = 0;
    idx_t options[METIS_NOPTIONS];
    idx_t objval;
    idx_t nZone = nPart;

    METIS_SetDefaultOptions(options);
    std::cout << "Now begining partition graph!\n";
    if (nZone > 8)
    {
        std::cout << "Using K-way Partitioning!\n";
        METIS_PartGraphKway(&nCells, &ncon, xadj.data(), adjncy.data(), vwgt, vsize, adjwgt.data(),
            &nZone, tpwgts, ubvec, options, &objval, cellzone.data());
    }
    else
    {
        std::cout << "Using Recursive Partitioning!\n";
        METIS_PartGraphRecursive(&nCells, &ncon, xadj.data(), adjncy.data(), vwgt, vsize, adjwgt.data(),
            &nZone, tpwgts, ubvec, options, &objval, cellzone.data());
    }
    std::cout << "The interface number: " << objval << std::endl;
    std::cout << "Partition is finished!\n";
}

void WritePartition(std::string & fname, idx_t* part, idx_t n, idx_t nparts)
{
    std::fstream file;
    std::ostringstream oss;
    oss << fname << ".part." << nparts;
    std::string filename = oss.str();
    file.open(filename.c_str(), std::ios_base::out );

    for ( idx_t i = 0; i < n; ++ i )
    {
        file << part[i] << "\n";
    }
    file.close();

}


int main(int argc, char *argv[])
{
    namespace fs = std::filesystem;

    std::cout << "Current working directory: " << fs::current_path() << '\n';

    std::fstream file;
    std::string filename = "../example_b.graph";
    file.open(filename);
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
    vidx_t part;
    xadj.resize(nvtxs + 1);
    adjncy.resize(2 * nedges);
    adjwgt.resize(2 * nedges);
    part.resize(nvtxs);

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
 
            if (edge < 1 || edge > nvtxs)
            {
                ;
            }

            int ewgt = 1;
            if ( readew ) {
                line = line.substr(pos);
                ewgt = std::stoi(line, &pos);
            }

            if ( k == nedges2 )
            {
                exit(0);
            }

            adjncy[k] = edge - 1;
            adjwgt[k] = ewgt;
            k++;

            line = line.substr(pos);
            if (line == "")
                break; // End of line
        }
        xadj[i + 1] = k;
    }

    file.close();
    int nparts = 4;
    //PartitionByMetis( nvtxs, xadj, adjncy, nparts, part);
    PartitionWeight(nvtxs, xadj, adjncy, adjwgt, nparts, part);
    WritePartition(filename, part.data(), nvtxs, nparts);

    return 0;
}