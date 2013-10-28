ShinobiCharts Screenshot
===============

The following header files include code that enables the user to take screenshots of their charts.

However, if you want to take a snapshot of your chart in iOS7, apple have added new functionality which makes it a lot easier take a snapshot of your chart.

For an explanation of apple's new snapshot functionality look at our blog post [HERE](http://www.shinobicontrols.com/blog/posts/2013/09/30/ios7-day-by-day-day-7-taking-snapshots-of-uiviews/)

To take a snapshot of your chart in versions less than iOS7 this header file offers a snapshot method which returns a UIImage of your chart. 

An example of how to use this method can be found below:

	UIImage *imageOfYourChart = [yourChart snapshot];
	
To add views to be drawn above the data on your chart you must use the "-addViewToSnapshot:addToGLView:" method.

An example of this would be to add Pie Chart labels to your snapshot image. This can be done by implementing the following SChartDelegate method:

	-(void)sChart:(ShinobiChart *)chart alterLabel:(UILabel *)label forDatapoint:(SChartRadialDataPoint *)datapoint atSliceIndex:(int)index inRadialSeries:(SChartRadialSeries *)series {
    	[_chart addViewToSnapshot:label addToGLView:YES];
	}

An explanation of this code snippet can be found in [this blog post](http://www.shinobicontrols.com/blog/posts/2012/03/26/taking-a-shinobichart-screenshot-from-your-app/). However, the blog post is slightly out of date so please bear with us while we update it.

If you have any trouble using this code snippet don't hesitate to get in touch at: info@shinobicontrols.com

License
-------

The [Apache License, Version 2.0](license.txt) applies to everything in this repository, and will apply to any user contributions.
