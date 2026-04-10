import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { toast, ToastContainer } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';
import {useNavigate} from "react-router-dom";
import AdminLoader from '../AdminLoader';
const AllOrganisingCommitteeMembers = () => {
  const [organisingMembers, setOrganisingMembers] = useState([]);
  const [priorityValues, setPriorityValues] = useState({});
  const [loading, setLoading] = useState(true);
  const token = localStorage.getItem('token');
  const navigate=useNavigate();

  useEffect(() => {
    const fetchMembers = async () => {
      setLoading(true);
      try {
        const response = await axios.get(
          `${import.meta.env.VITE_API_URL}/api/v1/committee?committee=organizing&page=1&limit=1000`,
          {
            headers:{
                Authorization: `Bearer ${token}`
            }
          }
        );
        setOrganisingMembers(response.data.data);
      } catch (error) {
        console.error('Error fetching members:', error);
        toast.error('Failed to fetch members. Please try again.');
      } finally {
        setLoading(false);
      }
    };
    fetchMembers();
  }, []);

  // Handle delete speaker
  const handleDelete = async (id) => {
    if (!token) {
      toast.error('Please log in first.');
      return;
    }

    try {
      const response = await axios.delete(
        `${import.meta.env.VITE_API_URL}/api/v1/committee/${id}`,
        {
          headers: { Authorization: `Bearer ${token}` },
        }
      );
      toast.success(response.data.message);
      // Remove the deleted speaker from the state
      setOrganisingMembers(orgMembers=>orgMembers.filter((member) => member.id !== id));
    } catch (error) {
      console.error('Error deleting member:', error);
      toast.error('Failed to delete member. Please try again.');
    }
  };

  // Handle set priority
  const handleSetPriority = async (id) => {
    if (!token) {
      toast.error('Please log in first.');
      return;
    }

    const priority = priorityValues[id];
    
    // Check if priority value is provided
    if (!priority || priority.trim() === '') {
      toast.error('Please enter a priority value.');
      return;
    }

    // Validate that it's a number
    const priorityNum = parseInt(priority, 10);
    if (isNaN(priorityNum)) {
      toast.error('Please enter a valid number for priority.');
      return;
    }

    const member = organisingMembers.find((m) => m.id === id);
    if (!member) {
      toast.error('Member not found in list. Please refresh.');
      return;
    }

    try {
      const formData = new FormData();
      formData.append('priority', String(priorityNum));
      formData.append('name', member.name);
      formData.append('specialization', member.specialization);
      formData.append('college', member.college);
      formData.append('committee', member.committee);
      if (member.position) {
        formData.append('position', member.position);
      } else {
        const match = String(member.description || '').match(/^Role:\s*(.+?)\s*\n/);
        if (match && match[1]) {
          formData.append('position', match[1]);
        }
      }
      formData.append('description', member.description);

      const response = await axios.put(
        `${import.meta.env.VITE_API_URL}/api/v1/committee/${id}`,
        formData,
        {
          headers: { Authorization: `Bearer ${token}` },
        }
      );
      toast.success(response.data.message || 'Priority updated successfully');
      
      // Clear the input field after successful update
      setPriorityValues(prev => ({ ...prev, [id]: '' }));
      
      // Optionally refresh the members list to reflect any changes
      const updatedResponse = await axios.get(
        `${import.meta.env.VITE_API_URL}/api/v1/committee?committee=organizing&page=1&limit=1000`,
        {
          headers: { Authorization: `Bearer ${token}` }
        }
      );
      setOrganisingMembers(updatedResponse.data.data);
    } catch (error) {
      console.error('Error setting priority:', error);
      toast.error('Failed to set priority. Please try again.');
    }
  };

  // Handle priority input change
  const handlePriorityInputChange = (id, value) => {
    setPriorityValues(prev => ({ ...prev, [id]: value }));
  };

  return (
    <div className="space-y-6">
      <div>
        <h2 className="admin-title">Organising Committee</h2>
        <p className="admin-muted mt-1">Manage organising committee members and priority.</p>
      </div>
      {loading ? (
        <div className="admin-card">
          <div className="admin-card-inner">
            <AdminLoader label="Loading members..." />
          </div>
        </div>
      ) : organisingMembers.length === 0 ? (
        <p className="text-center text-zinc-600 text-sm">No members found.</p>
      ) : (
        <div className="grid grid-cols-1 gap-4 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4">
          {organisingMembers.map((member) => (
            <div key={member.id} className="admin-card overflow-hidden flex flex-col">
              <div className="w-full h-52 bg-zinc-100 flex-shrink-0">
                <img
                  src={member.profile_picture_url}
                  alt={member.name}
                  className="w-full h-full object-cover"
                />
              </div>

              <div className="p-4 flex flex-col flex-grow">
                <h3 className="font-semibold text-base mb-2 text-zinc-900 line-clamp-2">{member.name}</h3>
                <p className="text-zinc-600 mb-2 text-sm line-clamp-2">
                  {member.specialization}
                </p>
                <p className="text-zinc-700 mb-3 font-medium text-sm line-clamp-2">
                  {member.college}
                </p>

                {/* Priority Section */}
                <div className="mb-3 p-3 rounded-lg border border-zinc-200 bg-zinc-50 flex-shrink-0">
                  <div className="flex justify-between items-center mb-1">
                    <label className="block text-xs font-medium text-zinc-700">
                      Set Priority
                    </label>
                    {member.priority && (
                      <span className="text-xs text-indigo-700 font-medium">
                        Current: {member.priority}
                      </span>
                    )}
                  </div>
                  <div className="flex gap-2">
                    <input
                      type="number"
                      placeholder="Priority"
                      value={priorityValues[member.id] || ''}
                      onChange={(e) => handlePriorityInputChange(member.id, e.target.value)}
                      className="admin-input py-1.5 text-xs"
                    />
                    <button
                      onClick={() => handleSetPriority(member.id)}
                      className="admin-button-primary px-3 py-1.5 text-xs"
                    >
                      Set
                    </button>
                  </div>
                </div>

                {/* Action Buttons */}
                <div className="flex gap-2 mt-auto">
                  <button
                    onClick={() => navigate(`/admin/all-organising-members/${member.id}`)}
                    className="admin-button-primary flex-1 py-2 text-xs"
                  >
                    Update
                  </button>
                  <button
                    onClick={() => handleDelete(member.id)}
                    className="admin-button-danger flex-1 py-2 text-xs"
                  >
                    Delete
                  </button>
                </div>
              </div>
            </div>
          ))}
        </div>
      )}
      <ToastContainer position="bottom-center" />
    </div>
  );
};

export default AllOrganisingCommitteeMembers;
