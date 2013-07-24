//  ShinobiChart+Screenshot.h
//  Created by Stuart Grey on 22/02/2012.
//
//  Copyright 2013 Scott Logic
// 
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#import <ShinobiCharts/ShinobiChart.h>
#import <ShinobiCharts/SChartCanvas.h>
#import "SChartGLView+Screenshot.h"

@interface ShinobiChart (Screenshot)
- (UIImage*)snapshot;
@end

@implementation ShinobiChart (Screenshot)

- (UIImage*)snapshot {
    
    CGRect glFrame = self.canvas.glView.frame;
    glFrame.origin.y = self.canvas.frame.origin.y;
    
    //Grab the GL screenshot
    UIImage *glImage = [self.canvas.glView snapshot];
    UIImageView *glImageView = [[UIImageView alloc] initWithFrame:glFrame];
    [glImageView setImage:glImage];
    
    //Grab the chart image (minus GL)
    if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
        UIGraphicsBeginImageContextWithOptions(self.frame.size, NO, [UIScreen mainScreen].scale);
    } else {
        UIGraphicsBeginImageContext(self.frame.size);
    }
    [self.layer renderInContext:UIGraphicsGetCurrentContext()];
    UIImage *chartImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    //Turn the chart image into a view and to create the composite
    UIImageView *chartImageView = [[UIImageView alloc] initWithFrame:self.frame];
    [chartImageView setClipsToBounds:YES];
    [chartImageView setImage:chartImage];
    
    //Add our GL capture to our chart capture
    [chartImageView addSubview:glImageView];
    
    //Grab the annotations
    for (SChartAnnotation *ann in [self getAnnotations]) {
        if (ann.position == SChartAnnotationAboveData && [self annotationIsOnScreen:ann.frame]) {
            //if above gl, then add to top of glImageView (annotations under gl are already part of the chartImageView)
            
            /* Certain annotations have transforms applied to them - see updateViewWithCanvas: of SChartAnnotationZooming. Due to this we have to ensure that the bounds are correct before starting (and that we have reset the transform).*/
            //get correct bounds of annotation
            CGRect correctBounds = ann.frame;
            correctBounds.origin = CGPointZero;
            CGContextRef context;
            
            //record annotations current bounds and transform
            CGRect oldBounds = ann.bounds;
            CGAffineTransform oldTransform = ann.transform;

            //set correct bounds and identity transform
            ann.bounds = correctBounds;
            ann.transform = CGAffineTransformIdentity;
            
            if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
                UIGraphicsBeginImageContextWithOptions(ann.frame.size, NO, [UIScreen mainScreen].scale);
            } else {
                UIGraphicsBeginImageContext(ann.frame.size);
            }
            context = UIGraphicsGetCurrentContext();
            
            [ann.layer renderInContext:context];
            
            CGRect annotationFrame = ann.frame;
            UIImage *annotationImage = UIGraphicsGetImageFromCurrentImageContext();
            UIImage *editedAnnotationImage = annotationImage;
            UIGraphicsEndImageContext();
            
            // Reset frame origin and into account the axis lines
            glFrame.origin = CGPointMake([self.yAxis.style.lineWidth floatValue],
                                         [self.yAxis.style.lineWidth floatValue]);
            glFrame.size = CGSizeMake(glFrame.size.width - (2 * [self.yAxis.style.lineWidth floatValue]),
                                      glFrame.size.height - (2 * [self.yAxis.style.lineWidth floatValue]));
            
            CGRect cropFrame = CGRectMake(0, 0, annotationFrame.size.width, annotationFrame.size.height);
            
            // If annotation frame is overlapping left or right side of chart
            if(CGRectGetMaxX(annotationFrame) > CGRectGetMaxX(glFrame)){
                annotationFrame = CGRectMake(annotationFrame.origin.x,
                                   annotationFrame.origin.y,
                                   CGRectGetMaxX(glFrame) - annotationFrame.origin.x,
                                   annotationFrame.size.height);
                cropFrame.size = CGSizeMake(annotationFrame.size.width, annotationFrame.size.height);
            }else if(annotationFrame.origin.x < glFrame.origin.x){
                cropFrame = CGRectMake(glFrame.origin.x - annotationFrame.origin.x,
                                       cropFrame.origin.y,
                                       CGRectGetMaxX(annotationFrame) - glFrame.origin.x,
                                       cropFrame.size.height);
                annotationFrame = CGRectMake(glFrame.origin.x,
                                             annotationFrame.origin.y,
                                             CGRectGetMaxX(annotationFrame) - glFrame.origin.x,
                                             annotationFrame.size.height);
            }
            
            // If annotation frame is overlapping top or bottom side of chart
            if(CGRectGetMaxY(annotationFrame) > CGRectGetMaxY(glFrame)){
                cropFrame.size = CGSizeMake(cropFrame.size.width, CGRectGetMaxY(glFrame) - annotationFrame.origin.y);
                annotationFrame = CGRectMake(annotationFrame.origin.x,
                                             annotationFrame.origin.y,
                                             annotationFrame.size.width,
                                             CGRectGetMaxY(glFrame) - annotationFrame.origin.y);
            }else if(annotationFrame.origin.y < glFrame.origin.y){
                cropFrame = CGRectMake(cropFrame.origin.x,
                                       glFrame.origin.y - annotationFrame.origin.y,
                                       cropFrame.size.width,
                                       CGRectGetMaxY(annotationFrame) - glFrame.origin.y);
                annotationFrame = CGRectMake(annotationFrame.origin.x,
                                             glFrame.origin.y,
                                             annotationFrame.size.width,
                                             CGRectGetMaxY(annotationFrame) - glFrame.origin.y);
            }
            
            // Crop image if it needs cropFrameping
            if(cropFrame.size.height != ann.frame.size.height || cropFrame.size.width != ann.frame.size.width){
                CGImageRef imageRef = CGImageCreateWithImageInRect([annotationImage CGImage], cropFrame);
                editedAnnotationImage = [UIImage imageWithCGImage:imageRef];
                CGImageRelease(imageRef);
            }
    
            //Turn the annotation image into a view and add to glImageView
            UIImageView *annImageView = [[UIImageView alloc] initWithFrame:annotationFrame];
            [annImageView setImage:editedAnnotationImage];
            [glImageView addSubview:annImageView];
            
            //set bounds and transform back
            ann.bounds = oldBounds;
            ann.transform = oldTransform;
        }
    }
    
    //Turn our composite into a single image
    UIGraphicsBeginImageContext(chartImageView.bounds.size);
    [chartImageView.layer renderInContext:UIGraphicsGetCurrentContext()];
    UIImage *completeChartImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return completeChartImage;
}

-(BOOL)annotationIsOnScreen:(CGRect)frame {
    if(CGRectGetMaxX(frame) >= 0 && frame.origin.x <= self.canvas.glView.frame.size.width){
        if(CGRectGetMaxY(frame) >= 0 && frame.origin.y <= self.canvas.glView.frame.size.height){
            return YES;
        }
    }
    return NO;
}

@end
