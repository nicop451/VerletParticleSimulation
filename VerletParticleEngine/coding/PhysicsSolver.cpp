#include "PhysicsSolver.hpp"

// SOLVER FUNCTIONS
   PhysicsSolver::PhysicsSolver(float iSimWidth, float iSimHeight, RenderWindow& iWin, int substeps, float iContraintRadius):substeps(substeps), simWidth(iSimWidth), simHeight(iSimHeight), constraintRadius(iContraintRadius), win(iWin)
   {
      grid.GridCellSize = 20; // Make it so later the sizing of this is dynamic and is updated each frame
      int rowAmount = simHeight/grid.GridCellSize;
      int columnAmount = simWidth/grid.GridCellSize;
      grid.setSize(rowAmount, columnAmount);
   }

   void PhysicsSolver::update(float deltaTime)
   {
      float subDt = deltaTime / substeps;
      for(int i = 0; i < substeps; i++)
      {
         Updategrid();
         addSpringForces();
         updatePositions(subDt);
         HandleCollisions();
      }
      //gravity = rotateVector(gravity, 0.01);
      displaySprings(win);
      displayParticles(win);  
   }



// PARTICLE FUNCTIONS
   void PhysicsSolver::addParticle(Vector2f iPos, Vector2f iVel, float iMass, float iRadius, bool iIsLocked, bool iIsHidden, Color iColor)
   {
      particleArray.emplace_back(VerletParticle(iPos, iVel, iMass, iRadius, iIsLocked, iIsHidden, particleArray.size(), iColor));
   }

   void PhysicsSolver::updatePositions(float deltaTime)
   {
      for(VerletParticle &ptc:particleArray)
      {
         ptc.update(gravity, deltaTime);
      }
   }

   void PhysicsSolver::displayParticles(RenderWindow& win)
   {
      for(VerletParticle &ptc:particleArray)
      {
         if(!ptc.isHidden)
         {
            win.draw(ptc.shape);
         }
      }
   }
   
   // COLLISION FUNCTIONS
      void PhysicsSolver::HandleCollisions()
      {

         int i = 0;
         for(Cell& c1:grid.GridContents)
         {
            if(c1.objectIndices.size() > 0)
            {

               int y = i / grid.columns;
               int x = i - y * grid.columns;
               
                  for(int dx = -1; dx <= 1; dx++)
                  {
                     if(x + dx >= 0 && x + dx < grid.columns)
                     {
                        for(int dy = -1; dy <= 1; dy++)
                        {
                           if(y + dy >= 0 && y + dy < grid.rows)
                           {
                              int cell2Index = grid.get(x + dx, y + dy);
                              Cell& c2 = grid.GridContents[cell2Index];

                              if(c2.objectIndices.size() > 0)
                              {
                                 checkCellCollisions(c1, c2);
                                    //std::cout << c2.objectIndices.size() << std::endl;
                              }
                           }
                           
                        }
                     }
                  }
                  
               }
               i++;
            }

         //grid.printContents();

         // Normal Particle - Particle Collision Naive algo
         int indexP1 = 0;
         int indexP2 = 0;
         for(VerletParticle &p1:particleArray)
         {
            p1.constrain(simWidth, simHeight, constraintRadius, dampening);
            // for(VerletParticle &p2:particleArray)
            // {
            //    if(indexP1 != indexP2) 
            //    {
            //       //fixPtcOvelap(p1, p2);
            //    }
            //    indexP2++;
            // }
            // indexP1++;
         }
      }

      void PhysicsSolver::fixPtcOvelap(VerletParticle& p1, VerletParticle& p2)
      {
         Vector2f baseVector = p1.pos - p2.pos;
         float sumOfRadii = p1.radius + p2.radius;
         float overlap = mag(baseVector) - sumOfRadii;
         if(overlap < 0)
         {
            float distToResolve = (overlap/sumOfRadii)/2;
            float massRatio = p1.mass/p2.mass;
            p1.pos = p1.pos - multVectConst(baseVector, distToResolve / massRatio);
            p2.pos = p2.pos + multVectConst(baseVector, distToResolve * massRatio);
         }
      }

      void PhysicsSolver::Updategrid()
      {
         //Clear Cells Particle Array from last frame
         grid.clear();
         // Fill Cells with Particles
         for(VerletParticle& ptc:particleArray)
         {
            unsigned int x = floor(ptc.pos.x/grid.GridCellSize);
            unsigned int y = floor(ptc.pos.y/grid.GridCellSize);
            // Add current particle to Cell
            grid.addObject(x, y, ptc.id);
         }
         //grid.printContents();
      }
      
      void PhysicsSolver::checkCellCollisions(Cell& c1, Cell& c2)
      {
         for(int id1:c1.objectIndices) // for every particle in first
         {
            for(int id2:c2.objectIndices) // Fix collision with every particle in other cell
            {
               if(id1 != id2)
               {
                  
                  VerletParticle& p1 = particleArray[id1];
                  VerletParticle& p2 = particleArray[id2];
                  fixPtcOvelap(p1, p2);
                  
               }
            }
         }
      }


// SPRING FUNCTIONS
   void PhysicsSolver::createParticleSpringGrid(int rowAmt, int columnAmt, float springSpacing, float iK, float iDamp)
   {
      for(int row = 0; row < rowAmt; row++)
      {
         for(int column = 0; column < columnAmt; column++)
         {
            Vector2f iPos = Vector2f((simWidth/2 + column * springSpacing) - (springSpacing*columnAmt)/2, (simHeight/2 + row * springSpacing) - 150);
            addParticle(iPos, Vector2f(0, 0), 15, 5, false, true, Color(255, 255, 255));
         }
      }

      int i = 0;
      for(VerletParticle &p1:particleArray)
      {
         int gridSize = rowAmt*columnAmt;
         float diagonalSpacing = mag(Vector2f(springSpacing, springSpacing));
         
         if((i + 1) % columnAmt != 0 && i < gridSize - columnAmt) // Across, Down, Diagonal
         {
            VerletParticle &p2 = particleArray[i + 1];
            VerletParticle &p3 = particleArray[i + rowAmt];
            VerletParticle &p4 = particleArray[i + rowAmt + 1];
            addSpring(p1, p2, springSpacing, iK, iDamp);
            addSpring(p1, p3, springSpacing, iK, iDamp);
            addSpring(p1, p4, diagonalSpacing, iK, iDamp);
         }

         if((i + 1) > columnAmt && (i + 1) % columnAmt != 0)
         {
            VerletParticle &p5 = particleArray[i - rowAmt];
            VerletParticle &p6 = particleArray[i - rowAmt + 1];
            addSpring(p1, p6, diagonalSpacing, iK, iDamp);
            addSpring(p1, p5, springSpacing, iK, iDamp);
         }

         if((i + 1) % columnAmt == 0 && i < gridSize - 1)
         {
            VerletParticle &p3 = particleArray[i + rowAmt];
            addSpring(p1, p3, springSpacing, iK, iDamp);
         }

         if((i + 1) > gridSize - columnAmt && i < gridSize - 1)
         {
            VerletParticle &p2 = particleArray[i + 1];
            addSpring(p1, p2, springSpacing, iK, iDamp);
         }
         i++;      
      }
   }

   void PhysicsSolver::addSpringForces()
   {
      for(Spring& spr:springArray)
      {
         spr.update();
      }
   }

   void PhysicsSolver::displaySprings(RenderWindow& win)
   {
      for(Spring& spr:springArray)
      {
         spr.display(win);
      }
   }

   void PhysicsSolver::addSpring(VerletParticle& p1, VerletParticle& p2, float initRestLength, float iK, float iDamp)
   {
      springArray.emplace_back(Spring(p1, p2, initRestLength, iK, iDamp));
   }












//