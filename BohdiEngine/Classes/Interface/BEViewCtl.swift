//
//  BEViewCtl.swift
//  Pods
//
//  Created by YuliSun on 27/03/2017.
//
//

import UIKit
import GLKit
import CoreMotion

protocol BEViewDelegate {
    func onBETouched(sender:Any)
}

class BEViewCtl: GLKViewController, UIGestureRecognizerDelegate {

    enum BECameraRotateMode {
        case AroundModelManual
        case AroundModelByGyroscope
        case RotateAR
    }
    
    var touchDelegate: BEViewDelegate?
    
    var glView: GLKView! {
        get {
            return self.view as! GLKView
        }
    }
    
    var glFrame: CGRect! {
        get {
            return self.view.frame
        }
    }
    
    private var _useTransparentBackground: Bool = false
    var useTransparentBackground: Bool! {
        get {
            return _useTransparentBackground
        }
        set {
            self.modalPresentationStyle = UIModalPresentationStyle.overCurrentContext
            self.navigationController?.modalPresentationStyle = UIModalPresentationStyle.overCurrentContext
            if newValue == true {
                self.view.isOpaque = false
                self.view.backgroundColor = UIColor.clear
                BESetClearScreenColor(0, 0, 0, 0)
            } else {
                self.view.isOpaque = true
                self.view.backgroundColor = UIColor.blue
                BESetClearScreenColor(0, 0, 1, 1)
            }
            _useTransparentBackground = newValue
        }
    }
    
    var useDeltaRotationData: Bool!
    
    private var _doesRotateCamera: Bool = false
    var doesRotateCamera: Bool! {
        get {
            return _doesRotateCamera
        }
        set {
            BESetDoesRotateCamera(newValue)
            _doesRotateCamera = newValue
        }
    }
    
    var doesDrawWireFrame: Bool!
    
    var deviceRotateMat3: CMRotationMatrix!
    var cameraRotateMode: BECameraRotateMode!
    
    var indicator: UIActivityIndicatorView!
    
    // MARK: private members
    private var director: UnsafeRawPointer
    private var pinch_scale: Float!
    private var motionManager: CMMotionManager!
    private var referenceAtt: CMAttitude!
    private var tap: UITapGestureRecognizer!
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    deinit {
        BETeardownGL()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        touchDelegate = nil
        pinch_scale   = 10.0
        motionManager = nil
        indicator     = nil
        useDeltaRotationData = false
        doesRotateCamera  = false
        doesDrawWireFrame = false
        cameraRotateMode  = BECameraRotateMode.RotateAR
        
        tap = UITapGestureRecognizer()
        tap.delegate = self
        self.view.addGestureRecognizer(tap)
        
        glviewSetup(with: self.glFrame)
    }

    private func glviewSetup(with frame:CGRect) {
        EAGLContext.setCurrent(EAGLContext.init(api: EAGLRenderingAPI.openGLES3))
        (self.view as! GLKView).context = EAGLContext.current()
        (self.view as! GLKView).drawableColorFormat = GLKViewDrawableColorFormat.RGBA8888
        (self.view as! GLKView).drawableDepthFormat = GLKViewDrawableDepthFormat.format24
        (self.view as! GLKView).drawableStencilFormat = GLKViewDrawableStencilFormat.format8
        (self.view as! GLKView).drawableMultisample = GLKViewDrawableMultisample.multisampleNone
        self.preferredFramesPerSecond = 60
        
        //setup BohdiEngine
        let width  = UInt32(self.view.bounds.width)
        let height = UInt32(self.view.bounds.height)
        BESetupGL(width, height)
    }
    
    override var canBecomeFirstResponder: Bool {
        get {
            return true
        }
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        self.becomeFirstResponder()
    }
    
    override func viewDidDisappear(_ animated: Bool) {
        self.resignFirstResponder()
        super.viewDidDisappear(animated)
    }
    
    override func motionEnded(_ motion: UIEventSubtype, with event: UIEvent?) {
        if motion == UIEventSubtype.motionShake {
            self.onClose(nil)
        }
    }
    
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

    // MARK: APIs
    func removeCurrentModel() {
        BERemoveCurrentModel()
    }
    
    func addModel(named modelName:String) {
        BEAddModelNamed(modelName)
    }
    
    //use default if names/name is null
    func addSkybox(names texnames:[String]?) {
        if let texs = texnames {
            BEAddSkyboxNames(texs[0], texs[1], texs[2],
                             texs[3], texs[4], texs[5])
        } else {
            BEAddSkyboxNames(nil, nil, nil,
                             nil, nil, nil)
        }
    }
    
    func addSkysph(named texname:String?) {
        if texname != nil {
            BEAddSkysphNamed(texname)
        } else {
            BEAddSkysphNamed(nil)
        }
    }
    
    func removeCurrentSkybox() {
        BERemoveCurrentSkybox()
    }
    
    func removeCurrentSkysph() {
        BERemoveCurrentSkysph()
    }
    
    func glviewResize(with frame:CGRect) {
        self.view.frame = frame
        let width  = UInt32(self.view.bounds.width)
        let height = UInt32(self.view.bounds.height)
        BEResizeGL(width, height)
    }
    
    //need 4x4 column major matrix
    func cameraReset(with mat4:[Float]) {
        BECameraReset(mat4[0],  mat4[1],  mat4[2],  mat4[3],
                      mat4[4],  mat4[5],  mat4[6],  mat4[7],
                      mat4[8],  mat4[9],  mat4[10], mat4[11],
                      mat4[12], mat4[13], mat4[14], mat4[15], false)
    }
    
    func cameraRotate(with mat3:GLKMatrix3, does incremental:Bool) {
        BECameraRotate(mat3.m00, mat3.m01, mat3.m02,
                       mat3.m10, mat3.m11, mat3.m12,
                       mat3.m20, mat3.m21, mat3.m22, incremental)
    }
    
    func cameraTranslate(with vec3:GLKVector3, does incremental:Bool) {
        BECameraTranslate(vec3.x, vec3.y, vec3.z, incremental)
    }
    
    //light pos follow camera if pos is null
    func lightReset(with position:GLKVector3) {
        BELightReset(position.x, position.y, position.z)
    }
    
    func handleGesture(pan offset:CGPoint) {
        BEPanGesture(Float(offset.x), Float(offset.y))
    }
    
    func handleGesture(pinch scale:Float) {
        BEPinchGesture(scale)
    }
    
    func startDeviceMotion() {
        self.motionManager = CMMotionManager();
        if !self.motionManager.isDeviceMotionActive {
            self.motionManager.deviceMotionUpdateInterval = 1.0/60.0;
            self.motionManager.startDeviceMotionUpdates(using: CMAttitudeReferenceFrame.xArbitraryZVertical);
        }
    }
    
    func stopDeviceMotion() {
        self.motionManager.stopDeviceMotionUpdates();
    }
    
    func onClose(_ sender:Any?) {
        self.dismiss(animated: true) { 
            //do something
        }
    }
}
