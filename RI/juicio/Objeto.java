package juicio;

public class Objeto {
	private double precision;
	private double recall;
	
	public Objeto(double p,double r){
		precision=p;
		recall=r;
	}
	public double getP(){
		return precision;
	}
	public double getR(){
		return recall;
	}
}
