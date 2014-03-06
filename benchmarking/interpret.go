package main

import (
	"bufio"
	"code.google.com/p/plotinum/plot"
	"code.google.com/p/plotinum/plotter"
	"code.google.com/p/plotinum/plotutil"
	"fmt"
	"log"
	"math"
	"os"
	"strconv"
	"strings"
)

type Statistics struct {
	N      float64
	Mean   float64
	StdDev float64
	Max    float64
	Min    float64
}

func max(a, b float64) float64 {
	if a >= b {
		return a
	}
	return b
}
func min(a, b float64) float64 {
	if a < b {
		return a
	}
	return b
}

func (s *Statistics) Calculate(values []float64) *Statistics {
	n, sum, ss, u, l := float64(len(values)), 0.0, 0.0, values[0], values[0]
	for _, x := range values {
		sum, ss = sum+x, ss+x*x
		u, l = max(u, x), min(l, x)
	}
	s.N, s.Mean = n, sum/n
	s.StdDev, s.Max, s.Min = math.Sqrt(ss/n-s.Mean*s.Mean), u, l
	return s
}

type Test struct {
	Name   string
	Values []float64
	Stats  *Statistics
}

func (t *Test) Calculate() *Statistics {
	t.Stats = (&Statistics{}).Calculate(t.Values)
	return t.Stats
}

func (t *Test) String() string {
	return fmt.Sprintf("%s: %v\n%v", t.Name, t.Values, t.Stats)
}

func ParseLine(line string) (test *Test, err error) {
	fields := strings.SplitN(strings.TrimSpace(line), ": ", 2)
	if len(fields) != 2 {
		err = fmt.Errorf("Invalid line")
		return
	}
	test = new(Test)
	test.Name = strings.Trim(fields[0], `"`)
	stringValues := strings.Split(strings.Trim(fields[1], `[]`), ",")
	test.Values = make([]float64, len(stringValues))
	for i, stringValue := range stringValues {
		test.Values[i], err = strconv.ParseFloat(stringValue, 64)
		if err != nil {
			return
		}
	}
	return
}

func (t *Test) ToXY(scale float64) plotter.XYs {
	xys := make(plotter.XYs, len(t.Values))
	for i, _ := range xys {
		xys[i].X, xys[i].Y = float64(i), t.Values[i]/scale
	}
	return xys
}

func main() {
	tests := []*Test{}
	// if file, err := os.Open("results.txt"); err != nil {
	if file, err := os.Stdin, error(nil); err != nil {
		log.Fatal(err)
	} else {
		scanner := bufio.NewScanner(file)
		for scanner.Scan() {
			test, err := ParseLine(scanner.Text())
			if err != nil {
				continue
			}
			test.Calculate()
			tests = append(tests, test)
		}
	}
	if len(tests) == 0 {
		log.Fatal("No tests found.")
	}
	if plt, err := plot.New(); err != nil {
		log.Fatal(err)
	} else {
		for i, test := range tests {
			xys := test.ToXY(100000.0)
			line, err := plotter.NewLine(xys)
			if err != nil {
				log.Fatal(err)
			}
			line.Color, line.Dashes = plotutil.Color(i), plotutil.Dashes(0)
			plt.Add(line)
			{
				name := fmt.Sprintf("%s (%d)", test.Name, int(test.Stats.Mean/100000))
				plt.Legend.Add(name, line)
			}
		}
		// plt.Legend.Font.Size = vg.Inches(plt.Legend.Font.Size * 0.75)
		plt.Legend.Font.Size *= 0.65
		plt.Legend.Top = true
		plt.Legend.Left = true
		// plt.Legend.YOffs = vg.Inches(-1.12)
		// plt.Legend.XOffs = vg.Inches(1)
		plt.HideX()
		// plt.X.Padding = 20
		plt.Y.Min = 0
		plt.Y.Max = 1600
		plt.Title.Text = fmt.Sprintf("Speed Test (averaged over %d runs and %d tests)", 100000, 100)
		plt.Y.Label.Text = "Cycles"
		plt.Save(6, 4, "number-parse.svg")
	}
}
