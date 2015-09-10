typedef matrix<double, 3, 1> sample_type;  // Trzy atrybuty.
typedef radial_basis_kernel<sample_type> kernel_type;
std::vector<sample_type> samples;
std::vector<double> labels;
// Dla czytelnoœci pominiêto kod ³adowania próbek.
randomize_samples(samples, labels);
svm_c_trainer<kernel_type> trainer;
double best_gamma = 0, best_C = 0, best_f2 = 0;

matrix<double> params = cartesian_product( 
  logspace(log10(1e-6), log10(100), 30), // gamma
  logspace(log10(100000), log10(1e-3), 30) // C
);

for (long col = 0; col < params.nc(); ++col)
{
    const double gamma = params(0, col);
    const double C = params(1, col);

    trainer.set_kernel(kernel_type(gamma));
    trainer.set_c(C);

    double f2  = cross_validation(trainer, samples, labels,
      10); // Walidacja 10-krotna.

    if (f2 > best_f2) {
      best_f2 = f2;
      best_gamma = gamma;
      best_C = C;
    }
}