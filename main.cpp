#include <iostream>
#include "ICP.h"
#include "io_obj.h"
#include <chrono>

int main (int argc, char const ** argv)
{   
    typedef double Scalar;
    typedef Eigen::Matrix<Scalar, 3, Eigen::Dynamic> Vertices;
    std::string file_source;
    std::string file_target;
    std::string file_source_reg;

    ///--- Option parsing    
    if(argc==1){
        std::cout << "examples: " << std::endl;
        std::cout << "./sparseicp data/hippo_0.obj data/hippo_1.obj data/hippo_0.obj__REG__hippo_1.obj" << std::endl;
        // std::cout << "./sparseicp data/bunny.obj data/bunny_cut2.obj data/bunny.obj__REG__bunny_cut2.obj" << std::endl;
        exit(0);
    }
    else if(argc==4)
    {
        file_source = argv[1];
        file_target = argv[2];
        file_source_reg = argv[3];        
    } 
    else 
    {
        std::cout << "argument error!" << std::endl;
        exit(0);
    }
    
    ///--- Model that will be rigidly transformed
    Vertices vertices_source;
    read_obj(vertices_source, file_source);
    std::cout << "source: " << vertices_source.rows() << "x" << vertices_source.cols() << std::endl;
    
    ///--- Model that source will be aligned to
    Vertices vertices_target;
    read_obj(vertices_target, file_target);
    std::cout << "target: " << vertices_target.rows() << "x" << vertices_target.cols() << std::endl;
    
    ///--- Execute registration
    auto tic = std::chrono::steady_clock::now(); 
        SICP::Parameters pars;
        pars.p = .5;
        pars.max_icp = 15;
        pars.print_icpn = true;
        SICP::point_to_point(vertices_source, vertices_target, pars);
    auto toc = std::chrono::steady_clock::now();

    ///--- Write result to file
    write_obj_replaceverts(file_source, vertices_source, file_source_reg);
    
    ///--- Print execution time
    double time_ms = std::chrono::duration <double, std::milli> (toc-tic).count();
    std::cout << "sparseicp registered source to target in: " << time_ms << "ms" << std::endl;
    return 0;
}
