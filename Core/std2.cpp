#include "std2.h"

std::string to_string(std::chrono::nanoseconds duration)
{
	double value;
	std::string unit;

	auto d = duration.count();

	if (d < 1'000) {
		value = static_cast<double>(d);
		unit = "ns";
	}
	else if (d < 1'000'000) {
		value = d / 1'000.0;
		unit = "us";
	}
	else if (d < 1'000'000'000) {
		value = d / 1'000'000.0;
		unit = "ms";
	}
	else {
		value = d / 1'000'000'000.0;
		unit = "s";
	}

	// Determine number of digits before the decimal
	int int_digits = (value < 1) ? 0 : static_cast<int>(std::log10(value)) + 1;
	int precision = std::max(0, 3 - int_digits);

	std::ostringstream oss;
	oss << std::fixed << std::setprecision(precision) << value << ' ' << unit;
	return oss.str();
}
