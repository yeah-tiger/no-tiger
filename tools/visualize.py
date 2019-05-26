# author: zzh
import xml.etree.cElementTree as ET
from graphviz import Digraph
import sys

def main():
    if len(sys.argv) < 2:
        print("error: no input file")
        print(f"example usage: {sys.argv[0]} input.xml")
        exit(1)
    nodeDict = dict()
    nextNodeIndex = 0
    edgeSet = list()
    tree = ET.ElementTree(file=sys.argv[1])
    root = tree.getroot()
    root.attrib['_parent'] = None
    root.attrib['_index'] = nextNodeIndex
    nextNodeIndex += 1
    nodes = list()
    nodes.append(root)
    while (nodes):
        node = nodes.pop(0)
        nodeDict[node.attrib['_index']] = node
        for childNode in node:
            childNode.attrib['_parent'] = node.attrib['_index']
            childNode.attrib['_index'] = nextNodeIndex
            nextNodeIndex += 1
            nodes.append(childNode)
            edge = (node.attrib['_index'], childNode.attrib['_index'])
            edgeSet.append(edge)

    g = Digraph('ast')
    for (index, node) in nodeDict.items():
        label = node.tag + ' ' + str(index)
        for attribName in node.attrib:
            if attribName.startswith('_'):
                continue
            attribValue = node.attrib[attribName]
            label = label + '\n' + attribName + ': '+ attribValue
        g.node(name=str(index), label=label)
    for edge in edgeSet:
        g.edge(str(edge[0]), str(edge[1]))
    g.view()


if __name__ == "__main__":
    main()