/*
    @author Thanos Vaskadiras
* Parallel implementation of the Barnes-Hut simulation
* basic code for the computation of the masses is based on the serial
* version of this algorithm made by chindesaurus
* time complexity is O (N log N)
*/
import java.awt.Color;
import java.io.FileInputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Locale;
import java.util.Scanner;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;



public class Nbodyparallel  implements Runnable  {

    static final int execTime = 100;
    static final boolean draw = true;
    private final int start;
    private final int end;
    private Body[] bodies;
    private final BHTree tree;
    private final Quad quad;
    private static double dt;

    public Nbodyparallel(Body[] bodies, BHTree tree, Quad quad, int start, int end){
        this.bodies = bodies;
        this.tree = tree;
        this.start = start;
        this.end = end;
        this.quad = quad;

    }



    public static void main(String[] args) throws FileNotFoundException {

        Locale.setDefault(new Locale("en", "US"));
        // input file for the sim to run on
        String fname ="inputs/saturnrings.txt";
        FileInputStream is = new FileInputStream(new File(fname));
        System.setIn(is);
        Scanner console = new Scanner(System.in);


        dt = 0.1;       //time quantum
        int N = console.nextInt();      // number of particles
        double radius = console.nextDouble();       // radius of universe

        if (draw){
            StdDraw.show(0);
            StdDraw.setXscale(-radius, +radius);
            StdDraw.setYscale(-radius, +radius);
        }

        Body[] bodies = new Body[N];
        for (int i=0; i < N; i++){
            double px = console.nextDouble();
            double py = console.nextDouble();
            double vx = console.nextDouble();
            double vy = console.nextDouble();
            double mass = console.nextDouble();
            int red = console.nextInt();
            int green = console.nextInt();
            int blue = console.nextInt();
            Color color = new Color(red, green, blue);
            bodies[i] = new Body(px, py, vx, vy, mass, color);
        }
        ThreadPoolExecutor executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(4);
        long startTime = System.nanoTime();
        System.out.println("Processors number: " + Runtime.getRuntime().availableProcessors());

        // begin simulation
        for (double t = 0.0; t < execTime; t = t +dt){
            Quad quad = new Quad(0,0,radius * 2);
            BHTree tree = new BHTree(quad);
            Nbodyparallel nbodyparallel = new Nbodyparallel(bodies, tree, quad,0, N );
            executor.execute(nbodyparallel);


            if (draw){
                StdDraw.clear(StdDraw.BLACK);
                for (int i=0; i < N; i++){
                    bodies[i].draw();
                }
                StdDraw.show(0);
            }
        }
        executor.shutdown();

        long endTime = System.nanoTime();
        long totalTime = endTime = startTime;
        System.out.println("Simulation time:" + ((double)totalTime)/1E9);
        System.exit(0);

    }


    @Override
    public void run() {
        try{
            for (int i = 0; i<end; i++){
                if (bodies[i].in(quad)){
                    tree.insert(bodies[i]);

                }
            }
            for (int i = 0; i < end; i++){
                bodies[i].resetForce();
                tree.updateForce(bodies[i]);
                bodies[i].update(dt);
            }
        }
        catch (Exception e) {
            e.printStackTrace();
    }
    }
}
