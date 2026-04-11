import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { toast, ToastContainer } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';
import {useNavigate} from "react-router-dom";
import AdminLoader from '../AdminLoader';

const AllIndustryProgrammeCommitteeMembers = () => {
  const [organisingMembers, setOrganisingMembers] = useState([]);
  const [loading, setLoading] = useState(true);
  const token = localStorage.getItem('token');
  const navigate=useNavigate();

  useEffect(() => {
    const fetchMembers = async () => {
      setLoading(true);
      try {
        const response = await axios.get(
          `${import.meta.env.VITE_API_URL}/api/v1/committee?committee=industry&page=1&limit=1000`,
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
      toast.success(response.data.msg || response.data.message || "Member deleted");
      // Remove the deleted speaker from the state
      setOrganisingMembers(orgMembers=>orgMembers.filter((member) => member.id !== id));
    } catch (error) {
      console.error('Error deleting member:', error);
      toast.error('Failed to delete member. Please try again.');
    }
  };

  return (
    <div className="space-y-6">
      <div>
        <h2 className="admin-title">Industry Committee</h2>
        <p className="admin-muted mt-1">Manage industry committee members.</p>
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
            <div key={member.id} className="admin-card overflow-hidden">
              <div className="w-full h-52 bg-zinc-100">
                <img
                  src={member.profile_picture_url}
                  alt={member.name}
                  className="w-full h-full object-cover"
                />
              </div>

              <div className="p-4">
                <h3 className="font-semibold text-base mb-2 text-zinc-900 line-clamp-2">{member.name}</h3>
                <p className="text-zinc-600 mb-4 text-sm line-clamp-2">
                  {member.specialization}
                </p>
                <p className="text-zinc-700 mb-4 text-sm">
                      <b>{member.college}</b>
                  </p>
                <div className="flex justify-between items-center">
                  <button
                    onClick={() => handleDelete(member.id)}
                    className="admin-button-danger"
                  >
                    Delete
                  </button>
                  <button
                    onClick={() => navigate(`/admin/all-industry-members/${member.id}`)}
                    className="admin-button-primary"
                  >
                    Update
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

export default AllIndustryProgrammeCommitteeMembers;
