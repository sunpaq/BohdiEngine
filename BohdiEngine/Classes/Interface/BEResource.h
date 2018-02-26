//
//  BEResource.h
//  BohdiEngine
//
//  Created by 孙御礼 on 2/25/30 H.
//

#import <Foundation/Foundation.h>

@interface BEResource : NSObject

@property (nonatomic) NSMutableArray<NSString*>* objModelNames;

+(instancetype)shared;

-(void) detectObjModelNames;
-(void) preloadJPGTextures;
-(void) preloadTGATextures;
-(void) preloadTextures:(NSString*)extension;

@end
