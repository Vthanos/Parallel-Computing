/**
 * Compilation:  javac NBodyBH.java
 * Execution:    java NBodyBH < inputs/[filename].txt
 * Dependencies: BHTree.java Body.java Quad.java StdDraw.java
 * Input files:  ./inputs/*.txt
 *
 * @author Thanos Vaskadiras
 * @version 1.00
 */

import java.awt.Color;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Locale;
import java.util.Scanner;


public class NBodyBH   {
    static final int execTime = 100;

    static final boolean draw = false;

    public static void main(String[] args) throws FileNotFoundException {

        Locale.setDefault(new Locale("en","Us"));

        String fname ="inputs/saturnrings.txt";
        FileInputStream is = new FileInputStream(fname);
        System.setIn(is);
        Scanner console = new Scanner(System.in);
        
        final double dt = 0.1;                     // time quantum
        int N = console.nextInt();                 // number of particles
        double radius = console.nextDouble();      // radius of universe

        // turn on animation mode and rescale coordinate system
        StdDraw.show(0);
        StdDraw.setXscale(-radius, +radius);
        StdDraw.setYscale(-radius, +radius);

        // read in and initialize bodies
        Body[] bodies = new Body[N];               // array of N bodies
        for (int i = 0; i < N; i++) {
            double px   = console.nextDouble();
            double py   = console.nextDouble();
            double vx   = console.nextDouble();
            double vy   = console.nextDouble();
            double mass = console.nextDouble();
            int red     = console.nextInt();
            int green   = console.nextInt();
            int blue    = console.nextInt();
            Color color = new Color(red, green, blue);
            bodies[i]   = new Body(px, py, vx, vy, mass, color);
        }
        long startTime = System.nanoTime();
        // simulate the universe
        for (double t = 0.0; t< execTime; t = t + dt) {

            Quad quad = new Quad(0, 0, radius * 2);
            BHTree tree = new BHTree(quad);

            // build the Barnes-Hut tree
            for (int i = 0; i < N; i++)
                if (bodies[i].in(quad))
                   tree.insert(bodies[i]);
        
            // update the forces, positions, velocities, and accelerations
            for (int i = 0; i < N; i++) {
                bodies[i].resetForce();
                tree.updateForce(bodies[i]);
                bodies[i].update(dt);
            }

            // draw the bodies
            StdDraw.clear(StdDraw.BLACK);
            for (int i = 0; i < N; i++)
                bodies[i].draw();

            StdDraw.show(10);
        }
        //Print execution time and exit PA
        long endTime = System.nanoTime();
        long totalTime = endTime - startTime;
        System.out.println("Simulation time: " + ((double) totalTime) / 1E9);
        System.exit(0);

    }

}
