#pragma once

#include <iostream>
#include <vector>

struct Cell
{
   static const unsigned int capacity = 4;
   std::vector<int> objectIndices = {};

   Cell() = default;

   void addObject(unsigned int id)
   {
      objectIndices.emplace_back(id);
   }
   
   void removeObject(unsigned int id)
   {
      for(int i{0}; i < objectIndices.size(); i++)
      {
         if(objectIndices[i] == id)
         {
            objectIndices[i] = objectIndices[objectIndices.size() - 1];
            objectIndices.resize(objectIndices.size() - 1);
            break;
         }
      }
   }

   void clear()
   {
      objectIndices.clear();
   }
};

struct Grid
{
   unsigned int rows;
   unsigned int columns;

   float GridCellSize;

   std::vector<Cell> GridContents = {};

   Grid() = default;


   void setSize(int iRows, int iColumns)
   {
      rows = iRows;
      columns = iColumns;
      GridContents.resize(iRows * iColumns);
   }

   int get(int x, int y)
   {
      int index = y * columns + x;
      return index;
   }

   void addObject(int x, int y, int id)
   {
      int index = y * columns + x;
      Cell& cell = GridContents[index];
      cell.addObject(id);
   }

   void removeObject(int x, int y, int id)
   {
      int index = y * columns + x;
      Cell& cell = GridContents[index];
      cell.removeObject(id);
   }

   void clear()
   {
      for(Cell& c:GridContents)
      {
         c.clear();
      }
   }

   void printContents()
   {
      std::cout << std::endl;
      int i{0};
      int TotalObjectCount{0};
      for(Cell& c:GridContents)
      {
         std::cout << "[";
         for(int id:c.objectIndices)
         {
            std::cout << " " << id << " ";
            TotalObjectCount++;
         }
         std::cout << "]";
         if((i + 1) % columns == 0)
         {
            std::cout << std::endl;
         }
         i++;
      }
      std::cout << std::endl << TotalObjectCount << " Objects " << std::endl << std::endl;
   }
};