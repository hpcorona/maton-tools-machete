//
//  analytics.m
//  machete
//
//  Created by Hilario Perez Corona on 3/30/12.
//  Copyright (c) 2012 Matón Supergames. All rights reserved.
//

#import "analytics.h"


#include "../analytics/analytics.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include <GANTracker.h>

namespace machete {
  namespace analytics {

GoogleTracker::GoogleTracker() {
  started = false;
}

GoogleTracker::~GoogleTracker() {
  if (started == true) {
    Stop();
  }
}

void GoogleTracker::Start(Str accountId, int dispatchPeriod) {
  if (started == true) {
    Stop();
  }
  
  char accountIdChars[20];
  accountId.GetChars(accountIdChars, 20);
  NSString * accId = [NSString stringWithUTF8String:accountIdChars] ;
  
  [[GANTracker sharedTracker] startTrackerWithAccountID:accId
                                         dispatchPeriod:dispatchPeriod
                                               delegate:nil];
  
  started = true;
}

void GoogleTracker::SetCustomVariable(int idx, Str name, Str value) {
  char nameChars[100];
  char valueChars[100];
  NSString *nsName;
  NSString *nsValue;
  
  name.GetChars(nameChars, 100);
  value.GetChars(valueChars, 100);
  
  nsName = [NSString stringWithUTF8String:nameChars];
  nsValue = [NSString stringWithUTF8String:valueChars];
  
  NSError *error = nil;
  [[GANTracker sharedTracker] setSampleRate:100];
  [[GANTracker sharedTracker] setAnonymizeIp:YES];
  [[GANTracker sharedTracker] setCustomVariableAtIndex:idx
                                                  name:nsName
                                                 value:nsValue
                                             withError:&error];
  
  if (error != nil) {
    NSLog(@"Error: %@", error);
  }
}

void GoogleTracker::TrackEvent(Str category, Str action, Str label, int value) {
  char categoryChars[100];
  char actionChars[100];
  char labelChars[100];
  
  NSString *nsCategory;
  NSString *nsAction;
  NSString *nsLabel;
  
  category.GetChars(categoryChars, 100);
  action.GetChars(actionChars, 100);
  label.GetChars(labelChars, 100);
  
  nsCategory = [NSString stringWithUTF8String:categoryChars];
  nsAction = [NSString stringWithUTF8String:actionChars];
  nsLabel = [NSString stringWithUTF8String:labelChars];

  NSError *error = nil;
  [[GANTracker sharedTracker] trackEvent:nsCategory
                                  action:nsAction
                                   label:nsLabel
                                   value:value
                               withError:&error];
  
  if (error != nil) {
    NSLog(@"Error: %@", error);
  }
}

void GoogleTracker::TrackPageview(Str page) {
  char pageView[100];
  
  NSString *nsPageview;
  
  page.GetChars(pageView, 100);
  
  nsPageview = [NSString stringWithUTF8String:pageView];

  NSError *error;
  [[GANTracker sharedTracker] trackPageview:nsPageview
                                  withError:&error];
  
  if (error != nil) {
    NSLog(@"Error: %@", error);
  }
}

void GoogleTracker::Stop() {
  [[GANTracker sharedTracker] stopTracker];
}

void GoogleTracker::Dispatch() {
  [[GANTracker sharedTracker] dispatch];
}

  }
}
