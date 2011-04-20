package com.maton.machete;

public class TickGenerator {
	private long prevTime;
	private long newTime;
	private boolean start = true;
	
	public TickGenerator() {
		start = true;
	}
	
	public void reset() {
		start = true;
	}
	
	public float nextFrame() {
		newTime = System.currentTimeMillis();
		
		if (start) {
			prevTime = newTime;
			start = false;
		}
		
		float diff = newTime - prevTime;
		if (diff > 100) {
			diff = 0;
		} else {
			diff = diff / 1000.0f;
		}
		
		prevTime = newTime;
		
		return diff;
	}
	
}
