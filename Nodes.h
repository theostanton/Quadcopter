void totNodes() {
  for( int k = 0; k < usedNodes; k++ ) {
    nodeTot[k] = node[k];
    node[k] = 0;
  }
}
