#include <assert.h>
#include <stdio.h>
#include <cmath>
#include <complex>

std::complex<float> I(0.0, 1.0);
float pi;

void dft(float in[], std::complex<float> out[], size_t n) 
{
	for (size_t f = 0; f < n; ++f) {
		out[f] = 0;
		for (size_t i = 0; i < n; ++i) {
			float t = (float)i / n;
			std::complex<float> z = I * (2 * pi * f * t);
			out[f] += in[i] * std::exp(z);
		}
	}
}

void fft(float in[], size_t stride, std::complex<float> out[], size_t n)
{
	assert(n > 0);

	if (n == 1) {
		out[0] = in[0];
		return;
	}

	fft(in, stride * 2, out, n / 2);
	fft(in + stride, stride * 2, out + n / 2, n / 2);

	for (size_t k = 0; k < n / 2; ++k) {
		float t = (float)k / n;
		std::complex<float> z = I * (-2 * pi * t);
		std::complex<float> v = std::exp(z)*out[k + n/2];
		std::complex<float> e = out[k];

		out[k] = e + v;
		out[k + n / 2] = e - v;
	}
}

int _main(void)
{
	pi = atan2f(1, 1) * 4;

	const size_t n = 8;
	float in[n];
	std::complex<float> out[n];

	for (size_t i = 0; i < n; ++i) {
		float t = (float)i / n;
		in[i] = cosf(2*pi*t*1) + sin(2*pi*t*2);
	}

	fft(in, 1, out, n);

	for (size_t f = 0; f < n; ++f) {
		printf("%02zu: %.2f %.2f\n", f, out[f].real(), out[f].imag());
	}

	return 0;
}