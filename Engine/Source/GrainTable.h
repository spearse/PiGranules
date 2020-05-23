/*
  ==============================================================================

    GrainTable.h
    Created: 13 Dec 2017 12:13:08pm
    Author:  stephen

  ==============================================================================
*/

#pragma once
#include <vector>
#include <math.h>
#include "../JuceLibraryCode/JuceHeader.h"


class Phasor
{
	float m_phase;
	float m_sr;
	float m_delta;
	float m_frequency;

public:
	Phasor(float sampleRate = 44100.0f, float frequency = 1.0f, float phase = 0.0f)
		: m_phase(phase)
		, m_sr(sampleRate)
		, m_delta(frequency / sampleRate)
		, m_frequency(frequency)
	{
	}

	float get_frequency() const { return m_frequency; }

	void set_frequency(float frequency)
	{
		m_frequency = frequency;
		m_delta = frequency / m_sr;
	}
	float get_phase() const { return m_phase; }

	void set_phase(float phase)
	{
		m_phase = phase;
	}

	// call me to tick the phasor forward by one sample.
	void tick()
	{
		m_phase += m_delta;
		if (m_phase < 0.0f)
		{
			m_phase += 1.0f;
		}
		else if (m_phase >= 1.0f)
		{
			m_phase -= 1.0f;
		}
	}
};


class GrainTable {
	long long int m_size;
	std::vector<float> m_table;
public:
	GrainTable(int size) :
		m_size(size),
		m_table(size)
	{
		fill_sine();
	}
	void fill_sine(float pitch = 1.0f) {

		for (int n = 0; n < m_size; ++n) {
			m_table[n] = sin((3.14159 * 2.0f * n * pitch) / m_size);
		}

	}
	void set_size(int size) {
		m_size = size;
		std::vector<float>  t(size);
		m_table = t;
	}
	void set_sample(int index, float sample) {
		if (index >= 0 && index < m_size) {
			m_table[index] = sample;
		}
	}
	void fill_hann() {

		for (int n = 0; n < m_size; ++n) {
			m_table[n] = 0.5 * (1 - cos(  (3.14159*2.0f*n)/m_size        ));
		}
	}
	int get_size() {
		return m_size - 1;
	}
	void fill_saw() {
		for (int n = 0; n < m_size; ++n) {
			m_table[n] = float(n) / m_size;
		}
	}
	void fill_inverse_saw() {
		for (int n = 0; n < m_size; ++n) {
			m_table[n] = float(m_size - n) / m_size;
		}
	}
	void fill_sawP(float pow) {
		for (int n = 0; n < m_size; ++n) {
			m_table[n] =  std::pow( float(n) / m_size,pow);
		}
	}
	void fill_inverses_sawP(float pow) {
		for (int n = 0; n < m_size; ++n) {
			m_table[n] = std::pow(float(m_size - n) / m_size, pow);
		}
	}
	void fill_openSquare() {
		for (int n = 0; n < m_size; ++n) {
			m_table[n] = 1;
		}
	}
	void fill_square(float pwidth) {
		for (int n = 0; n < m_size; ++n) {
			m_table[n] = n < m_size * pwidth ? 1.0 : 0;
		}
	}
	void fill_triangle() {
		for (int n = 0; n < m_size; ++n) {
			if (n < m_size*0.5) {
				m_table[n] = 2.0*(float(n ) / m_size);
			}
			else {
				m_table[n] =2.0f *(float(m_size-n) / m_size) ;
			}
		}
	}
private:
	double CubicInterpolate(
		double y0, double y1,
		double y2, double y3,
		double mu)
	{
		double a0, a1, a2, a3, mu2;

		mu2 = mu * mu;
		a0 = y3 - y2 - y0 + y1;
		a1 = y0 - y1 - a0;
		a2 = y2 - y0;
		a3 = y1;

		return(a0*mu*mu2 + a1 * mu2 + a2 * mu + a3);
	}
public:
    //crashes after load of new
	double get_sample(double index) {
		double fractpart, intpart;
		fractpart = modf(index, &intpart);
		long long int size = m_size - 1;

		long long int y0Index(intpart-1), y1Index(intpart), y2Index(intpart+1), y3Index(intpart+2);

		while (y0Index < 0)y0Index += size;
		while (y0Index > size)y0Index -= size;

		while (y1Index < 0)y1Index += size;
		while (y1Index > size)y1Index -= size;

		while (y2Index < 0)y2Index += size;
		while (y2Index > size)y2Index -= size;

		while (y3Index < 0)y3Index += size;
		while (y3Index > size)y3Index -= size;


		return CubicInterpolate( m_table[y0Index], m_table[y1Index], m_table[y2Index], m_table[y3Index],fractpart);

	}


};
