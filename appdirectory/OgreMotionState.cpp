class OgreMotionState : public btMotionState { 

protected: 
    Ogre::SceneNode* mObj; 
    btTransform mPos; 

public: 
    OgreMotionState(const btTransform &initialpos, Ogre::SceneNode* node) { 
        mObj = node; 
        mPos = initialpos; 
    } 
    virtual ~OgreMotionState() {} 
    //Provides flexibility in terms of object visibility 
    void setNode(Ogre::SceneNode* node) { 
        mObj = node; 
    } 
    
    virtual void getWorldTransform(btTransform &worldTrans) const { 
        worldTrans = mPos; 
    } 
    
    virtual void setWorldTransform(const btTransform &worldTrans) { 
        if (mVisibleobj == nullptr) 
            return; // silently return before we set a node 
        btQuaternion rot = worldTrans.getRotation(); 
        mVisibleobj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z()); 
        btVector3 pos = worldTrans.getOrigin(); 
        mVisibleobj->setPosition(pos.x(), pos.y(), pos.z()); 
    } 
}; 