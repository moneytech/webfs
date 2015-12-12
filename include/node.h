/**
 * \file node.h
 * \brief Definition of Node, NodeType and Node traversing functions
 */
#include <string>
#include <vector>

#include "rapidjson/writer.h"
#include "utils.h"
#include "file.h"


#ifndef WEBFS_NODE_H_
#define WEBFS_NODE_H_
namespace webfs {

/**
 * Main Node Data structure used to store local file metadata for mapping with
 * external storage
 */
class Node {
  public:
    enum class Type {
      BRANCH,
      LEAF,
    };

    Node(const std::string &n,const Type t):
      name(n),parent(nullptr),type(t){}

    /**
     * Add a child to the current Node
     */
    void addChild(Node *child);

    /**
     * Find the node at the provided relativePath (relative to the current node)
     * starting from the current Node.
     */
    Node *findChild(const std::string &relativePath);

    Node *findParent(const std::string &relativePath);

    const std::string& getName()const{
      return name;
    }

    const Type& getType()const {
      return type;
    }


    File *file;

  const std::vector<Node*> getChildren()const{
    return children;
  }

  /**
   * write the in a json format
   * @param out object where write the node
   */
  template<typename BufferType>
  void writeTo(rapidjson::Writer<BufferType> &out) const{
    out.StartObject();
	out.String("isLeaf");
	const bool isLeaf= type==Type::LEAF;
	out.Bool(isLeaf);
	out.String("name");
	out.String(name);
	out.String("id");
	out.String(remoteId);

	if(!isLeaf){
	  out.String("children");
	  out.StartArray();
	  for(const Node * n : children)
	    n->writeTo(out);
	  out.EndArray();
	}//if !isLeaf

	out.EndObject();
  }//writeTo

  void setRemoteId(const std::string &id){
	  remoteId=id;
  }


  private:

    Node* findInChildren(const std::string &currentName);
    std::string name;
    std::string remoteId;
    Node *parent;
    const Type type;
    std::vector<Node *> children;
}; //Node




} // webfs namespace
#endif // WEBFS_NODE_H_
