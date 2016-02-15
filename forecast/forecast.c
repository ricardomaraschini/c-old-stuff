#include <stdio.h>
#include <math.h>

double
weighted_average(double *demandv, int len)
{
	int	i = 0;
	int	j = 0;
	double	svals = 0;
	double	sweights = 0;

	for(i=0; i<len; i++) {
		j = i+1;
		svals += demandv[i]*j;
		sweights += j;
	}

	return svals / sweights;
}


/**
 * exponential smoothing:
 *
 * 	Ft+1 = α Dt + (1-α) Ft
 *	 	F = forecast
 * 		D = demand
 * 		α = smoothing constant
 *
 * on the example:
 * 	F₇ = α D₆ + (1 - α) F₆
 * 	...
 * 	F₂ = α D₁ + (1 - α) F₁
 *
 * α - smoothing constant must be between 0 and 1.
 *     if the behaviour is constant we should use a 
 *     high value for α. if the value of the demands
 *     changes constantly, we are advised to use a
 *     lower value for α;
 *
 *
 */
double
exponential_smoothing(double *demandv, int len)
{
	double	alpha = 0.2; // assumed
	int	i;
	double	sum = 0;
	double	forecast = 0;
	
	if (len <= 0)
		return 0;

	// assume initial forecast is the mathematical average
	for(i=0;i<len; i++) {
		sum += demandv[i];
	}
	forecast = sum / len;

	for(i=1; i<=len; i++)
		forecast = (alpha * demandv[i-1]) + (1-alpha) * forecast;

	return forecast;
}


/**
 * at+1 = α(dt+1 / ct+1) + (1-�)(at + bt)
 */

double
winters(double *demandv, int len, int season_len)
{

	return 0;
}

/**
 * holts
 *
 * Ft+1 = at + bt
 *
 * Ft+1 -> forecast
 * at   -> smoothed value(level) including the last data
 * bt   -> slope of the line given by bt
 */
double
holts(double *demandv, int len)
{
	double	alpha = 0.3;
	double	beta = 0.2;
	int	i = 0;
	double	prevat = 0;
	double	prevbt = 0;
	double	at = 0;
	double	bt = 0;

	if (len <= 0)
		return 0;

	prevbt = (demandv[len - 1] - demandv[0]) / (len - 1);
	prevat = demandv[0];
	for(i=1; i<len; i++) {
		at = (alpha * demandv[i]) + (1-alpha) * (prevat + prevbt);
		bt = beta * (at - prevat) + (1-beta) * prevbt;
		prevat = at;
		prevbt = bt;
	}

	return at + bt;
}


double
error_sum_of_squares(double *demandv, int len)
{
	int	i = 0;
	double	mean = 0;
	double	sum = 0;
	double	sst = 0;

	if (len == 0)
		return 0;

	for(i=0; i<len; i++) {
		sum += demandv[i];
	}
	mean = sum / len;

	for(i=0; i<len; i++) {
		sst += pow(demandv[i] - mean, 2);
	}

	return sst;
}

/**
 * while dealing with forecasting a behaviour that presents a trend,
 * we have to try to plot a line that are closer as possible to all
 * points in the graph.
 *
 * the gap between the point and the line is known as `error' and we
 * have to try to minimize it at most.
 *
 * technically speaking we have to assure a low `error sum of squares',
 * that is represented by:
 *
 *  ∑e²
 * (sum all squares of all errors, aka gaps)
 *
 * error on a given point can be noted as:
 *
 * et = yt - a - bt
 *
 * a and b are to be determined. putting the two formulas together we
 * have:
 *
 * ∑e² = ∑(yt - a - bt)²
 *
 * on this formula, a represents the slop and b represents the y axis
 * value on t = 0;
 *
 *
 *
 */


int
main(void)
{

	/**
	 *  level
	 * 	constant model
	 * 	F = a + ∑
	 *
	 * 	a = constant
	 *	∑ = noise on inherit variation
	 *		∑ is expected to have parameters (0, [sigma]^2)
	 *
	 *	the simplest way of forecasting is to detect the average
	 *	as being our constant a. but using this approach we invalidate
	 *	all entries and consider only the average, so all our next
	 *	forecasts will always be the same.
	 *
	 *	other approach is to provide weight to the samples, using more
	 *	representative weights on the recent data. if we call the weight
	 *	as `w', using the `demandv' as an example we woud have a constant
	 *	represented by the following formula:
	 *
	 *	(25*w1+32*w2+24*w3+28*w4+26*w5+27*w6) / (w1+w2+w3+w4+w5+w6)
	 *
	 * timeseries demand
	 * 	demand in respect to time
	 *	divided in:
	 *		- constant models
	 *		- trend models
	 *		- causal models
	 *
	 * trend
	 */
	
	double	const_demandv[] = { 25, 32, 24, 28, 26, 27 };
	double	trend_demandv[] = { 26, 28, 29, 31, 32, 35 };
	double	seaso_demandv[] = { 53, 22, 37, 45, 58, 25, 40, 50, 62, 27, 44, 56 };
	//printf("%.3f\n", weighted_average(const_demandv, 6));
	//printf("%.3f\n", error_sum_of_squares(trend_demandv, 6));
	
	printf("expsmooth constant:\t\t%.3f\n", exponential_smoothing(const_demandv, 6));
	//printf("expsmooth trend   :\t\t%.3f\n", exponential_smoothing(trend_demandv, 6));
	//printf("holts constant    :\t\t%.3f\n", holts(const_demandv, 6));
	printf("holts trend       :\t\t%.3f\n", holts(trend_demandv, 6));
	return 0;
}
