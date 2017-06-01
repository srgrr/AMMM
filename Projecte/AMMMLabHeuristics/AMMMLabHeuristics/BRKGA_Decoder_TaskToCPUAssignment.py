'''
AMMM Lab Heuristics v1.1
Specific BRKGA Decoder for Task To CPU Assignment.
Copyright 2016 Luis Velasco and Lluis Gifre.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''

from Solver_BRKGA import BRKGA_Decoder
from Solution import Solution

# The specific decoder for Task to CPU Assignment problem that implements methods in BRKGA_Decoder
# This decoder sorts the tasks to be assigned to CPUs according to a priority.
# The lower the priority, the earlier the task is assigned.
# Each gene in a chromosome represents the priority of a task for a specific individual.
# The decoder sorts the tasks acording to the priorities in the chromosome and
# tries to assign the tasks to CPUs following that order.
class TaskToCPUAssignment_Decoder(BRKGA_Decoder):
    def __init__(self, config, problem):
        self.config = config
        self.problem = problem
        
        numTasks = len(self.problem.getTasks())
        self.numIndividuals = numTasks * self.config.fIndividuals
        self.numGenes = numTasks
    
    def getNumIndividuals(self):
        return(self.numIndividuals)
    
    def getNumGenes(self):
        return(self.numGenes)

    def decode(self, individual):
        # get tasks and genes
        tasks = self.problem.getTasks()     # vector of tasks [t1, t2, t3, ...]
        genes = individual.chromosome       # vector of genes [g1, g2, g3, ...]
        
        # zip vector tasks and vector genes in a vector of tuples with the form:
        # [(t1,g1), (t2,g2), (t3,g3), ...]
        tasks_and_genes = zip(tasks, genes)
        
        # sort the vector of tuples tasks_and_genes by the genes (index 1 in the tuple)
        tasks_and_genes.sort(key=lambda task_and_gene: task_and_gene[0].getTotalResources() * task_and_gene[1], reverse=True)
        
        # create an empty solution for the individual
        individual.solution = Solution.createEmptySolution(self.config, self.problem)
        
        # assign each task following the given order
        for task_and_gene in tasks_and_genes:
            task = task_and_gene[0]
            taskId = task.getId()
            
            # get the feasible assignment for the current task with minimum highest CPU load
            assignment = individual.solution.findBestFeasibleAssignment(taskId)
            bestCPUId = assignment.cpuId
            
            if(bestCPUId is None):
                # infeasible solution
                individual.solution.makeInfeasible()
                break
            
            # do assignment
            individual.solution.assign(taskId, assignment.cpuId)
        
        individual.fitness = individual.solution.getHighestLoad()