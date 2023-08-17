def inputString = "元素1,元素2,元素3,元素4"
def elementsList = inputString.split(',')

for (element in elementsList) {
    println(element)
}

def node = "obstacale"

if (node in ["obstacle", "perception", "lane"]) {
    node = "perception"
}

println "node: $node"
