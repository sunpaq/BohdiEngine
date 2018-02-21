//
//  MainViewController.swift
//  BohdiEngine
//
//  Created by YuliSun on 31/05/2017.
//  Copyright © 2017 Sun YuLi. All rights reserved.
//

import UIKit

class MainViewController: UIViewController, BEViewRenderingDelegate {

    @IBOutlet weak var beview: BEView!
    @IBOutlet weak var beview2: BEView!
    @IBOutlet weak var panoview: BEPanoramaView!
    
    var gamectl: BEGameController!
    
    override func viewDidLoad() {
        super.viewDidLoad()

        beview.loadModelNamed("monkey2.obj")
        beview.renderer.doesAutoRotateCamera = false
        beview.delegate = self
        
        gamectl = BEGameController.shared()
        gamectl.startUsingGameController()
        gamectl.stickFactor = 10.0
        
        beview2.loadModelNamed("monkey2.obj")
        beview2.renderer.doesAutoRotateCamera = false
        
        panoview.loadPanoramaTexture("panorama360.jpg")
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        beview.startDraw3DContent(BECameraRotateAroundModelManual)//BECameraRotateAroundModelByGameController
        beview2.startDraw3DContent(BECameraRotateAroundModelManual)//BECameraRotateAroundModelManual
        panoview.startDraw()
    }

    func beforeRenderFrame() {
        beview.renderer.rotateModel(byPanGesture: gamectl.leftStick)
    }
    
    func afterRenderFrame() {
        
    }
    
    @IBAction func panAction(_ sender: UIPanGestureRecognizer) {
        beview.renderer.rotateModel(byPanGesture: sender.translation(in: beview))
        beview2.renderer.rotateModel(byPanGesture: sender.translation(in: beview))
    }
    
    @IBAction func pinchAction(_ sender: UIPinchGestureRecognizer) {
        beview.renderer.zoomModel(byPinchGesture: sender.scale)
        beview2.renderer.zoomModel(byPinchGesture: sender.scale)
    }
    
}
