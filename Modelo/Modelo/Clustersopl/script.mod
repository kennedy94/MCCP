main {
    
    var n_instances = 50;    //Numero de instancias
    var time_limit = 3600;    //Tempo de execucao de cada instancia em segundos
    var ofile = new IloOplOutputFile("resultados.txt");
    var source = new IloOplModelSource("Clusters.mod");
    var def = new IloOplModelDefinition(source);

    for(var j=1; j<=n_instances; j++){      	
        var model = new IloOplModel(def, cplex);
        var data = new IloOplDataSource("MCCP"+it+".dat");
        writeln("MCCP"+it+".dat");
        model.addDataSource(data);
        model.generate();
        writeln("Running instance "+j);
        cplex.tilim = time_limit;
        cplex.solve();
        writeln("Instance "+j+" ", "zIP = ", cplex.getObjValue(), " tIP = ", cplex.getSolvedTime());
        ofile.writeln("Instance "+j+" ", "zIP = ", cplex.getObjValue(),
                       " tzIP = ", cplex.getSolvedTime(), " GAP = ", cplex.getMIPRelativeGap()); 
    }

    data.end();
    model.end();
    def.end();
    source.end();
    ofile.close();
}