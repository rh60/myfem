
#define _USE_MATH_DEFINES 
#include "mfem.hpp"
#include <iostream>
#include <cmath>

using namespace mfem;
using namespace std;


double rhs(Vector& x)
{
    return 2 * sin(x[0]) * sin(x[1]);
}

double u2(Vector& x)
{
    return sin(x[0]) * sin(x[1]);
}

double u1(double x)
{
    return sin(x);
}

void SaveVTK(Mesh& mesh , GridFunction& x)
{
    ofstream ofs("solution.vtk");
    ofs.precision(8);
    const int ref = 1;
    mesh.PrintVTK(ofs, ref);
    x.SaveVTK(ofs, "u", ref);
}

void SaveVisit(Mesh& mesh, GridFunction& x)
{
    VisItDataCollection dc("solution", &mesh);
    dc.SetPrecision(8);
    dc.RegisterField("u", &x);
    dc.Save();
}

void dim1()
{
    Mesh mesh(10);
    mesh.Print(cout);
    int order = 2;
    H1_FECollection fec(order, mesh.Dimension());
    FiniteElementSpace fespace(&mesh, &fec);
    GridFunction u_exact(&fespace);
    GridFunction x(&fespace);
    mesh.GetNodes(x);
    for (size_t i = 0; i < x.Capacity(); i++)
    {
        u_exact[i] = u1(x[i]);
    }

    SaveVTK(mesh, u_exact);    
}

void dim2()
{
    int n1 = 30;

    Mesh mesh(n1, n1, Element::Type::TRIANGLE, 0, M_PI, M_PI);
    //mesh.Print(cout);
    int order = 2;
    H1_FECollection fec(order, mesh.Dimension());
    FiniteElementSpace fespace(&mesh, &fec);
    Array<int> ess_tdof_list;
    if (mesh.bdr_attributes.Size())
    {
        Array<int> ess_bdr(mesh.bdr_attributes.Max());
        ess_bdr = 1;
        fespace.GetEssentialTrueDofs(ess_bdr, ess_tdof_list);
    }

    cout << endl << "Essential Dofs" << endl;
    ess_tdof_list.Print(cout);

    LinearForm b(&fespace);
    ConstantCoefficient one(1.0);
    //ConstantCoefficient f(2.0);
    FunctionCoefficient f(rhs);
    b.AddDomainIntegrator(new DomainLFIntegrator(f));
    b.Assemble();

    BilinearForm a(&fespace);
    a.AddDomainIntegrator(new DiffusionIntegrator(one));
    a.Assemble();

    GridFunction x(&fespace);
    x = 0.0;

    OperatorPtr A;
    Vector B, X;
    a.FormLinearSystem(ess_tdof_list, x, b, A, X, B);

    auto S = (SparseMatrix&)(*A);

    GSSmoother M(S);
    PCG(*A, M, B, X, 1, 200, 1e-12, 0.0);

    a.RecoverFEMSolution(X, b, x);

    //save(mesh, x);

    GridFunction u_exact(&fespace);
    FunctionCoefficient u(u2);
    u_exact.ProjectCoefficient(u);
     
    GridFunction err(&fespace);
    for (size_t i = 0; i < err.Capacity(); i++)
    {
        err[i] = x[i] - u_exact[i];
    }
    SaveVisit(mesh, x);
}

int main()
{
    dim2();
}
