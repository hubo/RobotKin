/*
 -------------------------------------------------------------------------------
 Linkage.cpp
 Robot Library Project
 
 Initially created by Rowland O'Flaherty ( rowlandoflaherty.com ) on 5/11/13.
 
 Version 1.0
 -------------------------------------------------------------------------------
 */



//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "Linkage.h"
#include "Robot.h"


//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------
using namespace RobotKin;


//------------------------------------------------------------------------------
// Linkage Nested Classes
//------------------------------------------------------------------------------
// Link Class Constructors
Linkage::Link::Link()
:
    massProvided(false),
    tensorProvided(false)
{
}

Linkage::Link::Link(double mass, Eigen::Vector3d com, string name)
:
    massProvided(true),
    tensorProvided(false),
    mass_(mass),
    com_(com),
    name_(name)
{
}

Linkage::Link::Link(double mass, Eigen::Vector3d com, Eigen::Matrix3d inertiaTensor, string name)
:
    massProvided(true),
    tensorProvided(true),
    mass_(mass),
    com_(com),
    inertiaTensor_(inertiaTensor),
    name_(name)
{
}

// Link Destructor
Linkage::Link::~Link()
{
}

// Link Overloaded Operators


// Link Methods
string Linkage::Link::name() const { return name_; }
void Linkage::Link::name(string name)
{
    name_ = name;
}

double Linkage::Link::mass() const { return mass_; }
void Linkage::Link::mass(double mass)
{
    mass_ = mass;
}

Eigen::Vector3d Linkage::Link::com() const { return com_; }
void Linkage::Link::com(Eigen::Vector3d com)
{
    com_ = com;
}

Eigen::Matrix3d Linkage::Link::inertiaTensor() const { return inertiaTensor_; }
void Linkage::Link::inertiaTensor(Eigen::Matrix3d inertiaTensor)
{
    inertiaTensor_ = inertiaTensor;
}


// Joint Class Constructors
Linkage::Joint& Linkage::Joint::operator =( const Linkage::Joint& joint )
{
    respectToFixed_ = joint.respectToFixed_;
    respectToFixedTransformed_ = joint.respectToFixedTransformed_;

    name_ = joint.name_;
    id_ = joint.id_; // TODO: Should id really be changed?
    frameType_ = joint.frameType_;

    jointType_ = joint.jointType_;
    jointAxis_ = joint.jointAxis_;
    min_ = joint.min_;
    max_ = joint.max_;

    value(joint.value_);
}

Linkage::Joint::Joint(const Joint &joint)
    : Frame::Frame(joint.respectToFixed_, joint.name(), joint.id(), JOINT),
      respectToFixedTransformed_(joint.respectToFixedTransformed_),
      jointType_(joint.jointType_),
      jointAxis_(joint.jointAxis_),
      min_(joint.min_),
      max_(joint.max_),
      minVel_(joint.minVel_),
      maxVel_(joint.maxVel_),
      vel_(joint.vel_), // TODO: Should this be set like this?   
      minAcc_(joint.minAcc_),
      maxAcc_(joint.maxAcc_),
      acc_(joint.acc_), // TODO: Should this be set like this?
      link_(joint.link_)
{
    value(joint.value_);
}

Linkage::Joint::Joint(Eigen::Isometry3d respectToFixed,
                      string name,
                      size_t id,
                      JointType jointType,
                      Eigen::Vector3d axis,
                      double minValue,
                      double maxValue,
                      double minVel,
                      double maxVel,
                      double minAcc,
                      double maxAcc)
            : Frame::Frame(respectToFixed, name, id, JOINT),
              respectToFixedTransformed_(respectToFixed),
              respectToLinkage_(respectToFixed),
              jointType_(jointType),
              min_(minValue),
              max_(maxValue),
              minVel_(minVel),
              maxVel_(maxVel),
              minAcc_(minAcc),
              maxAcc_(maxAcc),
              value_(0),
              vel_(0),
              acc_(0)
{
    setJointAxis(axis);
    value(value_);
}


// Joint Destructor
Linkage::Joint::~Joint()
{

}

// Joint Overloaded Operators
const Linkage::Joint& Linkage::Joint::operator=(const double aValue)
{
    value(aValue);
    return *this;
}

void Linkage::Joint::setJointAxis(Eigen::Vector3d axis)
{
    jointAxis_ = axis;
    jointAxis_.normalize();
}

// Joint Methods
double Linkage::Joint::value() const { return value_; }
void Linkage::Joint::value(double value)
{
    value_ = value;
    if (jointType_ == REVOLUTE) {
        respectToFixedTransformed_ = respectToFixed_ * Eigen::AngleAxisd(value_, jointAxis_);
    } else if(jointType_ == PRISMATIC){
        respectToFixedTransformed_ = respectToFixed_ * Eigen::Translation3d(value_*jointAxis_);
    } else {
        respectToFixedTransformed_ = respectToFixed_;
    }

    // TODO: Decide if it is efficient to have this here
    if ( hasLinkage )
        linkage_->updateFrames();
}

double Linkage::Joint::max() const { return max_; }
void Linkage::Joint::max(double max)
{
  max_ = max;
}

double Linkage::Joint::min() const { return min_; }
void Linkage::Joint::min(double min)
{
  min_ = min;
}


// TODO: Check these velocity methods
double Linkage::Joint::vel() const { return vel_; }
void Linkage::Joint::vel(double vel)
{
  vel_ = vel;
}

double Linkage::Joint::maxVel() const { return maxVel_; }
void Linkage::Joint::maxVel(double maxVel)
{
  maxVel_ = maxVel;
}

double Linkage::Joint::minVel() const { return minVel_; }
void Linkage::Joint::minVel(double minVel)
{
  minVel_ = minVel;
}

// TODO: Check these acceleration methods
double Linkage::Joint::acc() const { return acc_; }
void Linkage::Joint::acc(double acc)
{
  acc_ = acc;
}

double Linkage::Joint::maxAcc() const { return maxAcc_; }
void Linkage::Joint::maxAcc(double maxAcc)
{
  maxAcc_ = maxAcc;
}

double Linkage::Joint::minAcc() const { return minAcc_; }
void Linkage::Joint::minAcc(double minAcc)
{
  minAcc_ = minAcc;
}


const Eigen::Isometry3d& Linkage::Joint::respectToFixed() const { return respectToFixed_; }
void Linkage::Joint::respectToFixed(Eigen::Isometry3d aCoordinate)
{
    respectToFixed_ = aCoordinate;
    value(value_);
}

const Linkage::Link& Linkage::Joint::const_link() const
{
    return link_;
}

Linkage::Link& Linkage::Joint::link()
{
    return link_;
}

const Eigen::Isometry3d& Linkage::Joint::respectToFixedTransformed() const
{
    return respectToFixedTransformed_;
}

const Eigen::Isometry3d& Linkage::Joint::respectToLinkage() const
{
    return respectToLinkage_;
}

Eigen::Isometry3d Linkage::Joint::respectToRobot() const
{
    if(hasLinkage)
        return linkage_->respectToRobot_ * respectToLinkage_;
    else
        return Eigen::Isometry3d::Identity();
}

Eigen::Isometry3d Linkage::Joint::respectToWorld() const
{
    if(hasLinkage)
        return linkage_->respectToWorld() * respectToLinkage_;
    else
        return Eigen::Isometry3d::Identity();
}

const Linkage* Linkage::Joint::linkage() const
{
    return linkage_;
}

void Linkage::Joint::printInfo() const
{
    cout << frameTypeString() << " Info: " << name() << " (ID: " << id()  << "), Joint Type:"
         << jointType_ << endl;
    cout << "Joint value: " << value() << "\t Axis: " << jointAxis_.transpose() << endl;
    cout << "Respect to fixed frame:" << endl;
    cout << respectToFixed().matrix() << endl << endl;
    cout << "Respect to fixed after transformation: " << endl;
    cout << respectToFixedTransformed().matrix() << endl << endl;
    cout << "Respect to linkage frame:" << endl;
    cout << respectToLinkage().matrix() << endl << endl;
    cout << "Respect to robot frame:" << endl;
    cout << respectToRobot().matrix() << endl << endl;
    cout << "Respect to world frame:" << endl;
    cout << respectToWorld().matrix() << endl << endl;
}


// Tool Class
Linkage::Tool& Linkage::Tool::operator =(const Linkage::Tool& tool)
{
    respectToFixed_ = tool.respectToFixed_;
    respectToLinkage_ = tool.respectToLinkage_;

    name_ = tool.name_;
    frameType_ = tool.frameType_;
}


Linkage::Tool::Tool(const Tool &tool)
    : Frame::Frame(tool.respectToFixed_, tool.name_, tool.id_, TOOL),
      respectToLinkage_(tool.respectToLinkage_)
{

}

Linkage::Tool::Tool(Eigen::Isometry3d respectToFixed, string name, size_t id)
    : Frame::Frame(respectToFixed, name, id, TOOL),
      respectToLinkage_(respectToFixed)
{

}

// Tool Destructor
Linkage::Tool::~Tool()
{
    
}

// Tool Methods
const Eigen::Isometry3d& Linkage::Tool::respectToFixed() const { return respectToFixed_; }
void Linkage::Tool::respectToFixed(Eigen::Isometry3d aCoordinate)
{
    respectToFixed_ = aCoordinate;
    if(hasLinkage)
        linkage_->updateFrames();
}

const Eigen::Isometry3d& Linkage::Tool::respectToLinkage() const
{
    return respectToLinkage_;
}

Eigen::Isometry3d Linkage::Tool::respectToRobot() const
{
    if(hasLinkage)
        return linkage_->respectToRobot_ * respectToLinkage_;
    else
        return respectToLinkage_;
}

Eigen::Isometry3d Linkage::Tool::respectToWorld() const
{
    if(hasLinkage)
        return linkage_->respectToWorld() * respectToLinkage_;
    else
        return respectToLinkage_;
}

const Linkage* Linkage::Tool::parentLinkage() const
{
    if(hasLinkage)
        return linkage_;
    else
        return NULL;
}

const Robot* Linkage::Tool::parentRobot() const
{
    if(hasRobot)
        return robot_;
    else
        return NULL;
}

size_t Linkage::Tool::getLinkageID()
{
    if(hasLinkage)
        return linkage_->id();
    else
        return 0;
}

string Linkage::Tool::getLinkageName()
{
    if(hasLinkage)
        return linkage_->name();
    else
        return "";
}

size_t Linkage::Tool::getRobotID()
{
    if(hasRobot)
        return robot_->id();
    else
        return 0;
}

string Linkage::Tool::getRobotName()
{
    if(hasRobot)
        return robot_->name();
    else
        return "";
}

size_t Linkage::Tool::getParentJointID()
{
    if(hasLinkage)
        if(linkage_->joints_.size() > 0)
            return linkage_->joints_.size()-1;

    return 0;
}

string Linkage::Tool::getParentJointName()
{
    if(hasLinkage)
        if(linkage_->joints_.size() > 0)
            return linkage_->joints().back()->name();

    return "";
}

size_t Linkage::Joint::getLinkageID()
{
    if(hasLinkage)
        return linkage_->id();
    else
        return 0;
}

string Linkage::Joint::getLinkageName()
{
    if(hasLinkage)
        return linkage_->name();
    else
        return "";
}

size_t Linkage::Joint::getRobotID()
{
    if(hasRobot)
        return robot_->id();
    else
        return 0;
}

string Linkage::Joint::getRobotName()
{
    if(hasRobot)
        return robot_->name();
    else
        return "";
}

size_t Linkage::getParentLinkageID()
{
    if(hasParent)
        return parentLinkage_->id();
    else
        return 0;
}

string Linkage::getParentLinkageName()
{
    if(hasParent)
        return parentLinkage_->name();
    else
        return "";
}

size_t Linkage::getRobotID()
{
    if(hasRobot)
        return robot_->id();
    else
        return 0;
}

string Linkage::getRobotName()
{
    if(hasRobot)
        return robot_->name();
    else
        return "";
}

void Linkage::getChildIDs(vector<size_t> &ids)
{
    ids.resize(childLinkages_.size());
    for(size_t i=0; i<childLinkages_.size(); i++)
        ids.push_back(childLinkages_[i]->id());
}

void Linkage::getChildNames(vector<string> &names)
{
    names.resize(childLinkages_.size());
    for(size_t i=0; i<childLinkages_.size(); i++)
        names.push_back(childLinkages_[i]->name());
}

void Linkage::printChildren()
{
    cout << "Children of " << name() << ":" << endl;
    for(size_t i=0; i<childLinkages_.size(); i++)
        cout << childLinkages_[i]->name() << " ("
             << childLinkages_[i]->id() << ")" << endl;
}

void Linkage::Tool::printInfo() const
{
    cout << frameTypeString() << " Info: " << name() << " (ID: " << id() << ")" << endl;
    cout << "Respect to fixed frame:" << endl;
    cout << respectToFixed().matrix() << endl << endl;
    cout << "Respect to linkage frame:" << endl;
    cout << respectToLinkage().matrix() << endl << endl;
    cout << "Respect to robot frame:" << endl;
    cout << respectToRobot().matrix() << endl << endl;
    cout << "Respect to world frame:" << endl;
    cout << respectToWorld().matrix() << endl << endl;
}

Linkage::Tool Linkage::Tool::Identity()
{
    Linkage::Tool tool;
    return tool;
}


//------------------------------------------------------------------------------
// Linkage Lifecycle
//------------------------------------------------------------------------------
// Constructors

Linkage& Linkage::operator =( const Linkage& linkage )
{
    respectToFixed_ = linkage.respectToFixed_;
    respectToRobot_ = linkage.respectToRobot_;
    
    name_ = linkage.name_;
    id_ = linkage.id_;
    frameType_ = linkage.frameType_;
    
    joints_.resize(0);
    for(size_t i=0; i<linkage.joints_.size(); i++)
        addJoint(*(linkage.joints_[i]));
    setTool(linkage.tool_);
    
    updateFrames();
}

Linkage::Linkage(const Linkage &linkage)
    : Frame::Frame(linkage.respectToFixed_, linkage.name_,
                   linkage.id_, linkage.frameType_),
      respectToRobot_(linkage.respectToRobot_),
      tool_(linkage.tool_),
      initializing_(false),
      hasParent(false),
      hasChildren(false)
{
    for(size_t i=0; i<linkage.joints_.size(); i++)
        addJoint(*(linkage.joints_[i]));

    setTool(linkage.tool_);
    
    updateFrames();
}

Linkage::Linkage()
    : Frame::Frame(Eigen::Isometry3d::Identity(), "", 0, LINKAGE),
      respectToRobot_(Eigen::Isometry3d::Identity()),
      initializing_(false),
      hasParent(false),
      hasChildren(false)
{
    analyticalIK = Linkage::defaultAnalyticalIK;
}

Linkage::Linkage(Eigen::Isometry3d respectToFixed, string name, size_t id)
    : Frame::Frame(respectToFixed, name, id, LINKAGE),
      respectToRobot_(Eigen::Isometry3d::Identity()),
      initializing_(false),
      hasParent(false),
      hasChildren(false)
{
    analyticalIK = Linkage::defaultAnalyticalIK;
}


Linkage::Linkage(Eigen::Isometry3d respectToFixed, string name, size_t id, Linkage::Joint joint, Linkage::Tool tool)
    : Frame::Frame(respectToFixed, name, id, LINKAGE),
      respectToRobot_(respectToFixed),
      initializing_(false),
      hasParent(false),
      hasChildren(false)
{
    analyticalIK = Linkage::defaultAnalyticalIK;
    vector<Linkage::Joint> joints(1);
    joints[0] = joint;
    initialize(joints, tool);
}

Linkage::Linkage(Eigen::Isometry3d respectToFixed, string name, size_t id, vector<Linkage::Joint> joints, Linkage::Tool tool)
    : Frame::Frame(respectToFixed, name, id, LINKAGE),
      respectToRobot_(respectToFixed),
      initializing_(false),
      hasParent(false),
      hasChildren(false)
{
    analyticalIK = Linkage::defaultAnalyticalIK;
    initialize(joints, tool);
}

// Destructor
Linkage::~Linkage()
{
    
}

//------------------------------------------------------------------------------
// Linkage Overloaded Operators
//------------------------------------------------------------------------------
// Assignment operator
const Linkage& Linkage::operator=(const Eigen::VectorXd& someValues)
{
    values(someValues);
    return *this;
}

//--------------------------------------------------------------------------
// Linkage Public Member Functions
//--------------------------------------------------------------------------

Linkage* Linkage::parentLinkage() { return parentLinkage_; }

size_t Linkage::nChildren() const { return childLinkages_.size(); }

size_t Linkage::nJoints() const { return joints_.size(); }

const Linkage::Joint& Linkage::const_joint(size_t jointIndex) const
{
    assert(jointIndex < nJoints());
    return *joints_[jointIndex];
}
const Linkage::Joint& Linkage::const_joint(string jointName) const { return *joints_[jointNameToIndex_.at(jointName)]; }

Linkage::Joint& Linkage::joint(size_t jointIndex)
{
    assert(jointIndex < nJoints());
    return *joints_[jointIndex];
}
Linkage::Joint& Linkage::joint(string jointName) { return *joints_[jointNameToIndex_.at(jointName)]; }

const vector<Linkage::Joint*>& Linkage::const_joints() const { return joints_; }
vector<Linkage::Joint*>& Linkage::joints() { return joints_; }


const Linkage::Tool& Linkage::const_tool() const { return tool_; }
Linkage::Tool& Linkage::tool() { return tool_; }


Eigen::VectorXd Linkage::values() const
{
    Eigen::VectorXd theValues(nJoints(),1);
    for (size_t i = 0; i < nJoints(); ++i) {
        theValues[i] = joints_[i]->value();
    }
    return theValues;
}

bool Linkage::values(const Eigen::VectorXd& someValues)
{    
    if(someValues.size() == nJoints())
    {
        for (size_t i = 0; i < nJoints(); ++i) {
            joints_[i]->value(someValues(i));
        }
        updateFrames();
        return true;
    }
    
    std::cerr << "ERROR! Number of values (" << someValues.size() << ") does not match "
              << "the number of joints (" << nJoints() << ")!" << std::endl;
    return false;
}

Eigen::VectorXd Linkage::minValues() const
{
    Eigen::VectorXd theMinValues(nJoints(),1);
    for (size_t i = 0; i < nJoints(); ++i) {
        theMinValues[i] = joints_[i]->min();
    }
    return theMinValues;
}

Eigen::VectorXd Linkage::maxValues() const
{
    Eigen::VectorXd theMaxValues(nJoints(),1);
    for (size_t i = 0; i < nJoints(); ++i) {
        theMaxValues[i] = joints_[i]->max();
    }
    return theMaxValues;
}

// TODO: Check these velocity methods
Eigen::VectorXd Linkage::vels() const
{
    Eigen::VectorXd theVels(nJoints(),1);
    for (size_t i = 0; i < nJoints(); ++i) {
        theVels[i] = joints_[i]->vel();
    }
    return theVels;
}

void Linkage::vels(const Eigen::VectorXd& someVels) {
    assert(someVels.size() == nJoints());    
    for (size_t i = 0; i < nJoints(); ++i) {
        joints_[i]->vel(someVels(i));
    }
    updateFrames();
}

Eigen::VectorXd Linkage::minVels() const
{
    Eigen::VectorXd theMinVels(nJoints(),1);
    for (size_t i = 0; i < nJoints(); ++i) {
        theMinVels[i] = joints_[i]->minVel();
    }
    return theMinVels;
}

Eigen::VectorXd Linkage::maxVels() const
{
    Eigen::VectorXd theMaxVels(nJoints(),1);
    for (size_t i = 0; i < nJoints(); ++i) {
        theMaxVels[i] = joints_[i]->maxVel();
    }
    return theMaxVels;
}

// TODO: Check these acceleration methods
Eigen::VectorXd Linkage::accs() const
{
    Eigen::VectorXd theAccs(nJoints(),1);
    for (size_t i = 0; i < nJoints(); ++i) {
        theAccs[i] = joints_[i]->acc();
    }
    return theAccs;
}

void Linkage::accs(const Eigen::VectorXd& someAccs) {
    assert(someAccs.size() == nJoints());    
    for (size_t i = 0; i < nJoints(); ++i) {
        joints_[i]->acc(someAccs(i));
    }
    updateFrames();
}

Eigen::VectorXd Linkage::minAccs() const
{
    Eigen::VectorXd theMinAccs(nJoints(),1);
    for (size_t i = 0; i < nJoints(); ++i) {
        theMinAccs[i] = joints_[i]->minAcc();
    }
    return theMinAccs;
}

Eigen::VectorXd Linkage::maxAccs() const
{
    Eigen::VectorXd theMaxAccs(nJoints(),1);
    for (size_t i = 0; i < nJoints(); ++i) {
        theMaxAccs[i] = joints_[i]->maxAcc();
    }
    return theMaxAccs;
}

const Eigen::Isometry3d& Linkage::respectToFixed() const { return respectToFixed_; }
void Linkage::respectToFixed(Eigen::Isometry3d aCoordinate)
{
    respectToFixed_ = aCoordinate;
    updateFrames();
}


const Eigen::Isometry3d& Linkage::respectToRobot() const
{
    return respectToRobot_;
}


Eigen::Isometry3d Linkage::respectToWorld() const
{
    if(hasRobot)
        return robot_->respectToWorld_ * respectToRobot_;
    else
        return Eigen::Isometry3d::Identity();
}

void Linkage::jacobian(Eigen::MatrixXd& J, const vector<Linkage::Joint*>& jointFrames, Eigen::Vector3d location, const Frame* refFrame) const
{ // location should be specified respect to linkage coordinate frame
    
    size_t nCols = jointFrames.size();
    J.resize(6, nCols);
    
    Eigen::Vector3d o_i, d_i, z_i; // Joint i location, offset, axis
    
    for (size_t i = 0; i < nCols; ++i) {
        
        o_i = jointFrames[i]->respectToLinkage_.translation(); // Joint i location
//        d_i = o_i - location; // Vector from location to joint i
        d_i = location - o_i; // Changing convention so that the position vector points away from the joint axis
//        z_i = jointFrames[i]->respectToLinkage_.rotation().col(2); // Joint i joint axis
        z_i = jointFrames[i]->respectToLinkage_.rotation()*jointFrames[i]->jointAxis_;

        // Set column i of Jocabian
        if (jointFrames[i]->jointType_ == REVOLUTE) {
//            J.block(0, i, 3, 1) = d_i.cross(z_i);
            J.block(0, i, 3, 1) = z_i.cross(d_i); // Changing convention to (w x r)
            J.block(3, i, 3, 1) = z_i;
        } else if(jointFrames[i]->jointType_ == PRISMATIC) {
            J.block(0, i, 3, 1) = z_i;
            J.block(3, i, 3, 1) = Eigen::Vector3d::Zero();
        } else {
            J.block(0, i, 3, 1) = Eigen::Vector3d::Zero();
            J.block(3, i, 3, 1) = Eigen::Vector3d::Zero();
        }
        
    }
    
    // Jacobian transformation
    Eigen::Matrix3d r(refFrame->respectToWorld().rotation().inverse() * respectToWorld().rotation());
    Eigen::MatrixXd R(6,6);
    R << r, Eigen::Matrix3d::Zero(), Eigen::Matrix3d::Zero(), r;
    J = R * J;
}


void Linkage::printInfo() const
{
    cout << frameTypeString() << " Info: " << name() << " (ID: " << id() << ")" << endl;
    
    cout << "Respect to fixed frame:" << endl;
    cout << respectToFixed().matrix() << endl << endl;
    cout << "Respect to robot frame:" << endl;
    cout << respectToRobot().matrix() << endl << endl;
    cout << "Respect to world frame:" << endl;
    cout << respectToWorld().matrix() << endl << endl;
    
    cout << "Joints (ID, Name, Value): " << endl;
    for (vector<Linkage::Joint*>::const_iterator jointIt = const_joints().begin();
         jointIt != const_joints().end(); ++jointIt) {
        cout << (*jointIt)->id() << ", " << (*jointIt)->name() << ", " << (*jointIt)->value() << endl;
    }
    
    for (vector<Linkage::Joint*>::const_iterator jointIt = const_joints().begin();
         jointIt != const_joints().end(); ++jointIt) {
        (*jointIt)->printInfo();
    }
    const_tool().printInfo();
    
    Eigen::MatrixXd J;
    jacobian(J, const_joints(), const_tool().respectToLinkage().translation(), this);
    
    cout << "Jacobian for " << name() << ":" << endl;
    cout << J.matrix() << endl;
    cout << "\n" << endl;
}

//------------------------------------------------------------------------------
// Linkage Private Member Functions
//------------------------------------------------------------------------------
void Linkage::initialize(vector<Linkage::Joint> joints, Linkage::Tool tool)
{
    initializing_ = true;
    
    for(size_t i = 0; i != joints.size(); ++i)
        addJoint(joints[i]);
    
    setTool(tool);
    
    initializing_ = false;

    updateFrames();
}

void Linkage::addJoint(Linkage::Joint newJoint)
{
    Joint* tempJoint = new Joint(newJoint);
    size_t newIndex = joints_.size();
    joints_.push_back(tempJoint);
    joints_[newIndex]->id_ = newIndex;
    joints_[newIndex]->linkage_ = this;
    joints_[newIndex]->hasLinkage = true;
    jointNameToIndex_[joints_[newIndex]->name()] = newIndex;
    
    if(hasRobot)
    {
        joints_[newIndex]->robot_ = robot_;
        joints_[newIndex]->hasRobot = true;
        
        robot_->joints_.push_back(joints_[newIndex]);
        robot_->joints_.back()->id_ = joints_.size()-1;
        jointNameToIndex_[joints_.back()->name()] = joints_.size()-1;
    }
}

void Linkage::setTool(Linkage::Tool newTool)
{
    tool_ = newTool;
    tool_.id_ = joints_.size();
    tool_.linkage_ = this;
    tool_.hasLinkage = true;

    if(hasRobot)
        tool_.Tool::robot_ = robot_;
    tool_.Tool::hasRobot = hasRobot;
}

void Linkage::setJointValue(size_t jointIndex, double val){joint(jointIndex).value(val);}

void Linkage::setJointValue(string jointName, double val){joint(jointName).value(val);}


void Linkage::updateFrames()
{
    if (~initializing_) {
        for (size_t i = 0; i < joints_.size(); ++i) {
            if (i == 0) {
                joints_[i]->respectToLinkage_ = joints_[i]->respectToFixedTransformed_;
                
            } else {
                joints_[i]->respectToLinkage_ = joints_[i-1]->respectToLinkage_ * joints_[i]->respectToFixedTransformed_;
            }
        }
        if(joints_.size() > 0)
            tool_.respectToLinkage_ = joints_[joints_.size()-1]->respectToLinkage_ * tool_.respectToFixed_;
        else
            tool_.respectToLinkage_ = tool_.respectToFixed_;
        
        if(hasChildren)
            updateChildLinkage();
    }
}


void Linkage::updateChildLinkage()
{
    for (size_t i = 0; i < nChildren(); ++i) {
        childLinkages_[i]->respectToRobot_ = tool_.respectToRobot() * childLinkages_[i]->respectToFixed_;
        if(childLinkages_[i]->hasChildren)
            childLinkages_[i]->updateChildLinkage();
    }
}

bool Linkage::defaultAnalyticalIK(Eigen::VectorXd& q, const Eigen::Isometry3d& B, const Eigen::VectorXd& qPrev) {
    // This function is just a place holder and should not be used. The analyticalIK function pointer should be set to the real analytical IK function.
    q = NAN * qPrev;
    return false;
}






