ShinobiCharts Screenshot
===============

The following header files include code that enables the user to take screenshots of their charts.

This method will return a UIImage of your chart and an example of how to use this method can be found below:

	UIImage *imageOfYourChart = [yourChart snapshot];
	
To add Pie Chart labels to the image of your chart you must implement the following SChartDelegate method:

	-(void)sChart:(ShinobiChart *)chart alterLabel:(UILabel *)label forDatapoint:(SChartRadialDataPoint *)datapoint atSliceIndex:(int)index inRadialSeries:(SChartRadialSeries *)series {
    	[_chart addPieChartLabel:label];
	}

An explanation of this code snippet can be found in a blog post [HERE](http://www.shinobicontrols.com/blog/posts/2012/03/26/taking-a-shinobichart-screenshot-from-your-app/). 

However, this blog post is slightly out of date and will be updated shortly.

If you have any trouble using this code snippet don't hesitate to get in touch at: info@shinobicontrols.com

License
-------

The [Apache License, Version 2.0](license.txt) applies to everything in this repository, and will apply to any user contributions.