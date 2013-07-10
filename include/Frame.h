/*
 -------------------------------------------------------------------------------
 Frame.h
 Robot Library Project
 
 CLASS NAME:
 Frame
 
 DESCRIPTION:
 description...
 
 FILES:
 Frame.h
 Frame.cpp
 
 DEPENDENCIES:
 
 
 CONSTRUCTORS:
 Frame();
 
 PROPERTIES:
 prop1 - description... .
 
 prop2 - description... .
 
 METHODS:
 type method1(type arg1);
 Description... .
 arg1 - description... .
 
 NOTES:
 
 
 EXAMPLES:
 Example 1: description
 ----------------------------------------------------------------------------
 code...
 ----------------------------------------------------------------------------
 
 
 VERSIONS:
 1.0 - 5/11/13 - Rowland O'Flaherty ( rowlandoflaherty.com )
 
 -------------------------------------------------------------------------------
 */



#ifndef _Frame_h_
#define _Frame_h_



//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <string>
#include <iostream>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>


//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------
using namespace std;


namespace RobotKin
{
    
    class Robot;
    class Linkage;
    
    //------------------------------------------------------------------------------
    // Typedefs
    //------------------------------------------------------------------------------
    
    enum FrameType {
        UNKNOWN,
        JOINT,
        TOOL,
        LINKAGE,
        ROBOT
    };
    
    
    
    class Frame
    {
        
        //--------------------------------------------------------------------------
        // Frame Friends
        //--------------------------------------------------------------------------
        friend class Linkage;
        friend class Robot;
        
    public:
        //--------------------------------------------------------------------------
        // Frame Destructor
        //--------------------------------------------------------------------------
        virtual ~Frame();
        
        
        //--------------------------------------------------------------------------
        // Frame Public Member Functions
        //--------------------------------------------------------------------------
        size_t id() const;
        
        string name() const;
        void name(string newName);
        
        FrameType frameType() const;
        string frameTypeString() const;
        
        virtual const Eigen::Isometry3d& respectToFixed() const = 0;
        virtual void respectToFixed(Eigen::Isometry3d aCoordinate) = 0;
        
        virtual Eigen::Isometry3d respectToWorld() const = 0;

        Eigen::Isometry3d respectTo(const Frame* aFrame) const;
        
        virtual void printInfo() const;
        
        
    protected:
        //--------------------------------------------------------------------------
        // Frame Constructor
        //--------------------------------------------------------------------------
        Frame(Eigen::Isometry3d respectToFixed = Eigen::Isometry3d::Identity(),
              string name = "",
              size_t id = 0,
              FrameType frameType = UNKNOWN);
        
        //--------------------------------------------------------------------------
        // Frame Protected Member Variables
        //--------------------------------------------------------------------------
        string name_;
        size_t id_;
        FrameType frameType_;
        Eigen::Isometry3d respectToFixed_; // Coordinates with respect to some fixed frame in nominal position

        Robot* robot_;
        Linkage* linkage_;

        bool hasRobot;
        bool hasLinkage;
        
    }; // class Frame
    
} // namespace RobotKin

#endif


